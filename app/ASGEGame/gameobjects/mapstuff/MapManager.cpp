//
// Created by Norbert / Matteo on 15/04/2022.
//

#include "MapManager.hpp"
#include <Engine/Logger.hpp>
#include <algorithm>
#include <iostream>
#include <tmxlite/TileLayer.hpp>
#include <utility>

#include "../../helper/ZOrderData.hpp"

#include "gameobjects/mapstuff/collectables/consumables/MedKit.hpp"

// Map Objects
#include "../../../../app/ASGEGame/gameobjects/mapstuff/bush/Bush.hpp"
#include "../../../../app/ASGEGame/gameobjects/mapstuff/camera/SecurityCamera.hpp"
#include "../../../../app/ASGEGame/gameobjects/mapstuff/collectables/consumables/MedKit.hpp"
#include "../../../../app/ASGEGame/gameobjects/mapstuff/collectables/consumables/MissionObjectiveObject.hpp"
#include "../../../../app/ASGEGame/gameobjects/mapstuff/collectables/key/Key.hpp"
#include "../../../../app/ASGEGame/gameobjects/mapstuff/collectables/weapons/Pistol.hpp"
#include "../../../../app/ASGEGame/gameobjects/mapstuff/collectables/weapons/Rifle.hpp"
#include "../../../../app/ASGEGame/gameobjects/mapstuff/computer/ComputerTerminal.hpp"
#include "../../../../app/ASGEGame/gameobjects/mapstuff/doors/AutomaticDoor.hpp"
#include "../../../../app/ASGEGame/gameobjects/mapstuff/doors/LockedDoor.hpp"
#include "../../../../app/ASGEGame/gameobjects/mapstuff/doors/ManualDoor.hpp"
#include "../../../../app/ASGEGame/gameobjects/mapstuff/endpoint/EndPoint.hpp"
#include "../../../../app/ASGEGame/gameobjects/mapstuff/tripwire/TripWire.hpp"
#include "../../../../app/ASGEGame/gameobjects/mapstuff/vent/Vent.hpp"
#include "gameobjects/characters/nonplayercharacters/Guard.hpp"
#include "gameobjects/mapstuff/alarm/AlarmManager.hpp"

/// Tile Layers:
/// Background
/// MapBorders
/// Floor
/// Walls
/// Windows
/// Decorations
/// CameraHit

/// Object Layers:
/// PlayersSpawnPositions
/// GuardPath1
/// GuardPath2
/// GuardPath3
/// GuardPath4
/// GuardPath5
/// VentPair1
/// VentPair2
/// VentPair3
/// HealthPickUps
/// WeaponPickups
/// KeyPickups
/// EndPoint
/// Bushes
/// Terminals
/// Cameras
/// Alarms
/// Tripwires
/// BreakableWallsH
/// BreakableWallsV
/// OpenDoorsH
/// OpenDoorsV
/// HackDoorsH
/// HackDoorsV
/// LockedDoorsH
/// LockedDoorsV
/// EndLevelItem

using driver = bcw::Driver;

MapManager::MapManager(int mag, std::string level) : map_name(std::move(level)), magnification(mag)
{
  initMap();
}

void MapManager::initMap()
{
  if (tile_map.open(DEFAULT_MAP_FOLDER + map_name))
  {
    buffer = tile_map.read();
    std::string file_string(buffer.as_char(), buffer.length);

    map.loadFromString(file_string, ".");

    map_size = ASGE::Point2D{ float(map.getTileCount().x * map.getTileSize().x),
                              float(map.getTileCount().y * map.getTileSize().y) };

    path_finder =
      std::make_unique<bcw::PathFinder>(map_size.x / getTileSize(), map_size.y / getTileSize());

    alarm_manager = bcw::Driver::getSceneHandler()->addScene<AlarmManager>();

    /// Puts each tile sets first GID into a vector. The size of that vector
    /// shows how many different tile sets the map uses.
    auto tile_sets = map.getTilesets();
    for (const auto& tile_set : tile_sets)
    {
      tileset_GIDs.push_back(tile_set.getFirstGID());
    }

    for (const auto& layer : map.getLayers())
    {
      int16_t layer_count = ZOrderData::MAP;
      /// This branch works with the Tile layers, calling the builder method
      if (layer->getType() == tmx::Layer::Type::Tile)
      {
        buildTiles(*layer, layer_count);
        ++layer_count;
      }
      /// This branch deals with the object group layers, fetching the player
      /// start positions and the guard patrol nodes
      else if (layer->getType() == tmx::Layer::Type::Object)
      {
        buildObjects(*layer);
      }
    }

    initHackableDoors();
  }
  std::dynamic_pointer_cast<AlarmManager>(alarm_manager)->init();
}

/// Builds all the tiles that belong to a specific layer.
void MapManager::buildTiles(tmx::Layer& layer, int16_t z)
{
  auto tile_layer = layer.getLayerAs<tmx::TileLayer>();
  int y           = 0;
  for (unsigned int row = 0; row < layer.getSize().y; ++row)
  {
    int x = 0;
    for (unsigned int col = 0; col < layer.getSize().x; ++col)
    {
      auto tile_info   = tile_layer.getTiles()[row * tile_layer.getSize().x + col];
      const auto* tile = map.getTilesets()[getTileSet(tile_info.ID)].getTile(tile_info.ID);

      if (tile != nullptr)
      {
        auto& tile_temp =
          tiles.emplace_back(std::make_unique<TileSquare>(tile, magnification, x, y, z));
        bcw::Vector2 pos{ static_cast<float>(x), static_cast<float>(y) };

        if (layer.getName() == "MapBorder" || layer.getName() == "Windows" || layer.getName() == "Walls")
        {
          tile_temp->setToBlocking();
          tile_temp->addCollider();
          addBlockToPathfinder(x, y);
          collision_tiles.emplace_back(tile_temp.get());
        }
        /// Doesn't exist a Pathfinder layer yet
        /*else if (layer.getName() == "Pathfinder")
        {
          // TODO: Map files to have a layer that stores blocking terrain data
          addBlockToPathfinder(x, y);
        }*/
      }
      x += tile_size * magnification;
    }
    y += tile_size * magnification;
  }
}

/// Creates the game objects based on the map data, using tmx data layer information
void MapManager::buildObjects(tmx::Layer& layer)
{
  auto object_layer = layer.getLayerAs<tmx::ObjectGroup>();
  auto objects      = object_layer.getObjects();

  for (const auto& object : objects)
  {
    bcw::Vector2 pos{ object.getPosition().x, object.getPosition().y };

    if (layer.getName() == "PlayersSpawnPositions")
    {
      player_starting_locations.push_back(pos);
    }
    else if (layer.getName() == "GuardPath1")
    {
      guard_one_path.push_back(pos);
      if (guard_one_path.size() == objects.size())
      {
        auto guard1 = bcw::Driver::getSceneHandler()->addGameObject<Guard>();
        guard1->init(guard_one_path.front(), this);
        guard1->setAlarmManager(std::dynamic_pointer_cast<AlarmManager>(alarm_manager));
        guard1->setPatrolRoute(guard_one_path);
      }
    }
    else if (layer.getName() == "GuardPath2")
    {
      guard_two_path.push_back(pos);
      if (guard_two_path.size() == objects.size())
      {
        auto guard2 = bcw::Driver::getSceneHandler()->addGameObject<Guard>();
        guard2->init(guard_two_path.front(), this);
        guard2->setAlarmManager(std::dynamic_pointer_cast<AlarmManager>(alarm_manager));
        guard2->setPatrolRoute(guard_two_path);
      }
    }
    else if (layer.getName() == "GuardPath3")
    {
      guard_three_path.push_back(pos);
      if (guard_three_path.size() == objects.size())
      {
        auto guard3 = bcw::Driver::getSceneHandler()->addGameObject<Guard>();
        guard3->init(guard_three_path.front(), this);
        guard3->setAlarmManager(std::dynamic_pointer_cast<AlarmManager>(alarm_manager));
        guard3->setPatrolRoute(guard_three_path);
      }
      // Logging::DEBUG("Guard3 should be here");
    }
    else if (layer.getName() == "GuardPath4")
    {
      guard_four_path.push_back(pos);
      if (guard_four_path.size() == objects.size())
      {
        auto guard4 = bcw::Driver::getSceneHandler()->addGameObject<Guard>();
        guard4->init(guard_four_path.front(), this);
        guard4->setAlarmManager(std::dynamic_pointer_cast<AlarmManager>(alarm_manager));
        guard4->setPatrolRoute(guard_four_path);
      }
      // Logging::DEBUG("Guard4 should be here");
    }
    else if (layer.getName() == "GuardPath5")
    {
      guard_five_path.push_back(pos);
      if (guard_five_path.size() == objects.size())
      {
        auto guard5 = bcw::Driver::getSceneHandler()->addGameObject<Guard>();
        guard5->init(guard_five_path.front(), this);
        guard5->setAlarmManager(std::dynamic_pointer_cast<AlarmManager>(alarm_manager));
        guard5->setPatrolRoute(guard_five_path);
      }
      // Logging::DEBUG("Guard5 should be here");
    }
    else if (layer.getName() == "VentPair1")
    {
      Logging::DEBUG("VentPair1 should be here");
    }
    else if (layer.getName() == "VentPair2")
    {
      Logging::DEBUG("VentPair2 should be here");
    }
    else if (layer.getName() == "VentPair3")
    {
      Logging::DEBUG("VentPair3 should be here");
    }
    else if (layer.getName() == "HealthPickups")
    {
      auto med_kit = bcw::Driver::getSceneHandler()->addGameObject<MedKit>();
      med_kit->init(pos);
      // med_kit->getSprite()->setGlobalZOrder(15);
      // Logging::DEBUG("Medkit should be here");
    }
    else if (layer.getName() == "WeaponPickups")
    {
      auto rifle = bcw::Driver::getSceneHandler()->addGameObject<Rifle>();
      rifle->init(pos);
      // rifle->getSprite()->setGlobalZOrder(15);
      // Logging::DEBUG("Weapons should be here");
    }
    else if (layer.getName() == "KeyPickups")
    {
      auto key = bcw::Driver::getSceneHandler()->addGameObject<Key>();
      key->init(pos, IKeyUnlockable::KeyColour::YELLOW); // temp maybe idk

      // Logging::DEBUG("Keys should be here");
    }
    else if (layer.getName() == "EndPoint")
    {
      auto end_point = bcw::Driver::getSceneHandler()->addGameObject<EndPoint>();
      end_point->init(pos, { 32 * 6, 32 * 2 }, true, 0.0F);
      // end_point->getSprite()->setGlobalZOrder(15);
      // Logging::DEBUG("End *point should be here");
    }
    else if (layer.getName() == "Bushes")
    {
      auto bush = bcw::Driver::getSceneHandler()->addGameObject<Bush>();
      bush->init(pos);
      // bush->getSprite()->setGlobalZOrder(15);
      // Logging::DEBUG("Bushes should be here");
    }
    else if (layer.getName() == "Alarms")
    {
      auto alarm = bcw::Driver::getSceneHandler()->addGameObject<Alarm>();
      alarm->init(pos);
    }
    else if (layer.getName() == "Cameras")
    {
      auto camera = bcw::Driver::getSceneHandler()->addGameObject<SecurityCamera>();
      camera->init(
        pos, 0.F, -45.F, 45.F, 0.2F, std::dynamic_pointer_cast<AlarmManager>(alarm_manager));
      // camera->getSprite()->setGlobalZOrder(15);
      // Logging::DEBUG("Cameras should be here");
    }
    else if (layer.getName() == "Tripwires")
    {
      auto tripwire = bcw::Driver::getSceneHandler()->addGameObject<TripWire>();
      tripwire->init(pos);
      // tripwire->getSprite()->setGlobalZOrder(15);
      // Logging::DEBUG("Tripwire should be here");
    }
    else if (layer.getName() == "OpenDoorsH")
    {
      auto open_door = bcw::Driver::getSceneHandler()->addGameObject<ManualDoor>();
      open_door->init(
        "/data/images/doors/open_door/OpenDoorFront.png", pos, { pos.x - 64, pos.y }, false, false);
      collision_doors.emplace_back(open_door->getSprite());
      // Logging::DEBUG("Open Doors Horiziontal should be here");
    }
    else if (layer.getName() == "OpenDoorsV")
    {
      auto open_door = bcw::Driver::getSceneHandler()->addGameObject<ManualDoor>();
      open_door->init(
        "/data/images/doors/open_door/OpenDoorLeft.png", pos, { pos.x, pos.y - 64 }, false, false);
      collision_doors.emplace_back(open_door->getSprite());
      // Logging::DEBUG("Open Doors Vertical should be here");
    }
    else if (layer.getName() == "HackDoorsH")
    {
      std::shared_ptr<bcw::Scene> hack_door =
        bcw::Driver::getSceneHandler()->addGameObject<AutomaticDoor>();
      hack_doors.emplace_back(hack_door, pos);
      // Logging::DEBUG("Hack Doors Horiziontal should be here");
    }
    else if (layer.getName() == "HackDoorsV")
    {
      std::shared_ptr<bcw::Scene> hack_door =
        bcw::Driver::getSceneHandler()->addGameObject<AutomaticDoor>();
      hack_doors.emplace_back(hack_door, pos);
      // Logging::DEBUG("Hack Doors Vertical should be here");
    }
    else if (layer.getName() == "LockedDoorsH")
    {
      auto door = bcw::Driver::getSceneHandler()->addGameObject<LockedDoor>();
      door->init(
        "/data/images/doors/locked_door/LockedDoorFront.png",
        pos,
        { pos.x - 64, pos.y },
        false,
        true);
      door->setUnlockColour(IKeyUnlockable::KeyColour::YELLOW);
      collision_doors.emplace_back(door->getSprite());
      // Logging::DEBUG("Locked Doors Horiziontal should be here");
    }
    else if (layer.getName() == "LockedDoorsV")
    {
      auto door = bcw::Driver::getSceneHandler()->addGameObject<LockedDoor>();
      door->init(
        "/data/images/doors/locked_door/LockedDoorLeft.png",
        pos,
        { pos.x, pos.y - 64 },
        false,
        true);
      door->setUnlockColour(IKeyUnlockable::KeyColour::YELLOW);
      collision_doors.emplace_back(door->getSprite());
      // Logging::DEBUG("Locked Doors Vertical should be here");
    }
    else if (layer.getName() == "Terminals")
    {
      std::shared_ptr<bcw::Scene> terminal =
        bcw::Driver::getSceneHandler()->addGameObject<ComputerTerminal>();
      hack_door_terminals.emplace_back(terminal, pos);
      // Logging::DEBUG("Terminals should be here");
    }
    else if (layer.getName() == "EndLevelItem")
    {
      auto end_level_item = bcw::Driver::getSceneHandler()->addGameObject<MissionObjectiveObject>();
      end_level_item->init(pos);
    }
    /*else if (layer.getName() == "BreakableWallsH"){} /// Not yet implemented */
    /*else if (layer.getName() == "BreakableWallsV"){} /// Not yet implemented */
    else
    {
      Logging::ERRORS("Failed to load " + layer.getName());
    }
  }
}

uint16_t MapManager::getTileSet(uint32_t tile_id)
{
  auto cont_size = static_cast<uint16_t>(tileset_GIDs.size());

  /// If the map uses more than one tile sets, check which one to use for
  /// the specified tile ID.
  if (cont_size > 1)
  {
    for (uint16_t i = cont_size - 1; i > 0; --i)
    {
      if (tile_id > tileset_GIDs[i])
      {
        return i;
      }
    }
  }
  return 0;
}

std::vector<std::unique_ptr<TileSquare>>& MapManager::getTiles()
{
  return tiles;
}

float MapManager::getTileSize() const
{
  return static_cast<float>(tile_size * magnification);
}

bool MapManager::isPassable(unsigned int index)
{
  return tiles[index]->isPassable();
}

ASGE::Point2D MapManager::checkCollision(
  const ASGE::Sprite& sprite, float radius, float speed, ASGE::Point2D vector,
  const ASGE::GameTime& game_time)
{
  // ASGE::Point2D to_return = vector;
  // auto obj_bounds = sprite.getWorldBounds();
  bcw::Vector2 currentpos = { sprite.midpoint().x, sprite.midpoint().y};

  bcw::Vector2 direction = { vector.x, vector.y };
  bcw::Vector2 new_position;

  new_position.x =
    currentpos.x + (direction.x * speed * static_cast<float>(game_time.fixedTsInSecs()));
  new_position.y =
    currentpos.y + (direction.y * speed * static_cast<float>(game_time.fixedTsInSecs()));

  bcw::Vector2 actual_np;
  actual_np.x =
    sprite.xPos() + (direction.x * speed * static_cast<float>(game_time.fixedTsInSecs()));
  actual_np.y =
    sprite.yPos() + (direction.y * speed * static_cast<float>(game_time.fixedTsInSecs()));
  auto maximum = [&](float a, float b) { return ((a < b) ? b : a); };
  auto minimum = [&](float a, float b) { return ((b < a) ? b : a); };

  for (auto& tile : collision_tiles)
  {
    if (tile->getDistance(sprite.midpoint()) < 100.0)
    {
      bcw::Vector2 nearest_point;
      nearest_point.x =
        maximum(tile->getBounds().v1.x, minimum(new_position.x, tile->getBounds().v2.x));
      nearest_point.y =
        maximum(tile->getBounds().v1.y, minimum(new_position.y, tile->getBounds().v3.y));
      // Logging::DEBUG("v1.x :" + std::to_string(tile->getBounds().v1.x) + "v1.y" +
      // std::to_string(tile->getBounds().v1.y) + "v3.x:" +std::to_string(tile->getBounds().v3.x) +
      // "v3.y" + std::to_string(tile->getBounds().v3.y) );

      bcw::Vector2 ray_nearest;
      ray_nearest.x = nearest_point.x - new_position.x;
      ray_nearest.y = nearest_point.y - new_position.y;

      float overlap = sprite.width() * 0.5F - ray_nearest.magnitude();

      if (std::isnan(overlap))
      {
        overlap = 0;
      }

      // tile->checkCollision()
      if (overlap > 0)
      {
        new_position.x = new_position.x - ray_nearest.normalise().x * overlap;
        new_position.y = new_position.y - ray_nearest.normalise().y * overlap;
        actual_np.x    = actual_np.x - ray_nearest.normalise().x * overlap;
        actual_np.y    = actual_np.y - ray_nearest.normalise().y * overlap;
      }
    }
  }

  for (auto& door : collision_doors)
  {
    bcw::Vector2 nearest_point;
    nearest_point.x =
      maximum(door->getWorldBounds().v1.x, minimum(new_position.x, door->getWorldBounds().v3.x));
    nearest_point.y =
      maximum(door->getWorldBounds().v1.y, minimum(new_position.y, door->getWorldBounds().v3.y));
    // Logging::DEBUG("v1.x :" + std::to_string(tile->getBounds().v1.x) + "v1.y" +
    // std::to_string(tile->getBounds().v1.y) + "v3.x:" +std::to_string(tile->getBounds().v3.x) +
    // "v3.y" + std::to_string(tile->getBounds().v3.y) );

    bcw::Vector2 ray_nearest;
    ray_nearest.x = nearest_point.x - new_position.x;
    ray_nearest.y = nearest_point.y - new_position.y;

    float overlap = sprite.width() * 0.5F - ray_nearest.magnitude();

    if (std::isnan(overlap))
    {
      overlap = 0;
    }

    // tile->checkCollision()
    if (overlap > 0)
    {
      new_position.x = new_position.x - ray_nearest.normalise().x * overlap;
      new_position.y = new_position.y - ray_nearest.normalise().y * overlap;
      actual_np.x    = actual_np.x - ray_nearest.normalise().x * overlap;
      actual_np.y    = actual_np.y - ray_nearest.normalise().y * overlap;
    }
  }
  return ASGE::Point2D{ actual_np.x, actual_np.y };
  /*
      if (
        getDistance(
          tile->getBounds(),
          predictBounds(obj_bounds, static_cast<float>(speed * game_time.deltaInSecs()), vector)) <
        45.26F)
      {
        if (tile->checkCollision(predictBounds(
              obj_bounds, static_cast<float>(speed * game_time.deltaInSecs()), { vector.x, 0.0F })))
        {
          to_return.x = 0.0F;
        }
        if (tile->checkCollision(predictBounds(
              obj_bounds, static_cast<float>(speed * game_time.deltaInSecs()), { 0.0F, vector.y })))
        {
          to_return.y = 0.0F;
        }
      }

    }*/
}

void MapManager::renderMap()
{
  for (auto& tile : tiles)
  {
    tile->renderTile();
  }
}

ASGE::Point2D MapManager::getPlayerStartPos(uint16_t idx)
{
  return player_starting_locations[idx];
}

ASGE::Point2D MapManager::getMapSize()
{
  return map_size;
}

float MapManager::getDistance(ASGE::SpriteBounds obj1, ASGE::SpriteBounds obj2)
{
  ASGE::Point2D point1{ obj1.v1.x + (obj1.v2.x - obj1.v1.x) * 0.5F,
                        obj1.v1.y + (obj1.v4.y - obj1.v1.y) * 0.5F };
  ASGE::Point2D point2{ obj2.v1.x + (obj2.v2.x - obj2.v1.x) * 0.5F,
                        obj2.v1.y + (obj2.v4.y - obj2.v1.y) * 0.5F };

  return static_cast<float>(sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2)));
}

ASGE::SpriteBounds
MapManager::predictBounds(ASGE::SpriteBounds curr_bound, float speed_dt, ASGE::Point2D vector)
{
  ASGE::SpriteBounds to_return = curr_bound;
  to_return.v1.x += static_cast<float>(std::round(vector.x * speed_dt));
  to_return.v1.y += static_cast<float>(std::round(vector.y * speed_dt));
  to_return.v2.x += static_cast<float>(std::round(vector.x * speed_dt));
  to_return.v2.y += static_cast<float>(std::round(vector.y * speed_dt));
  to_return.v3.x += static_cast<float>(std::round(vector.x * speed_dt));
  to_return.v3.y += static_cast<float>(std::round(vector.y * speed_dt));
  to_return.v4.x += static_cast<float>(std::round(vector.x * speed_dt));
  to_return.v4.y += static_cast<float>(std::round(vector.y * speed_dt));

  return to_return;
}

void MapManager::addBlockToPathfinder(int x, int y)
{
  path_finder->setBlockingProperty(
    x / static_cast<int>(getTileSize()), y / static_cast<int>(getTileSize()), true);
}

/// Interface between the game scene and the pathfinder.
/// Returns a path from the start location to the destination, uses a
/// cost map stored in the the PathFinder class
std::deque<bcw::Vector2> MapManager::findPath(ASGE::Point2D start, ASGE::Point2D destination)
{
  return path_finder->findPath(
    static_cast<size_t>(start.x / getTileSize()),
    static_cast<size_t>(start.y / getTileSize()),
    static_cast<size_t>(destination.x / getTileSize()),
    static_cast<size_t>(destination.y / getTileSize()));
}

void MapManager::initHackableDoors()
{
  for (size_t i = 0; i < hack_door_terminals.size(); ++i)
  {
    std::shared_ptr<ComputerTerminal> terminal =
      std::dynamic_pointer_cast<ComputerTerminal>(hack_door_terminals[i].first);
    std::shared_ptr<AutomaticDoor> door =
      std::dynamic_pointer_cast<AutomaticDoor>(hack_doors[i].first);

    bcw::Vector2 door_pos = hack_doors[i].second;
    door->init(
      "/data/images/doors/hack_door/HackDoorFront.png",
      door_pos,
      { door_pos.x - 50, door_pos.y },
      false,
      false);
    collision_doors.emplace_back(door->getSprite());

    bcw::Vector2 computer_pos = hack_door_terminals[i].second;
    terminal->init(computer_pos, { std::dynamic_pointer_cast<IHackable>(door) });
  }
}
/*
void MapManager::initMapStringLayer()
{
  mapStringLayer["PlayersSpawnPositions"] = PLAYER_SPAWN_POSITIONS;
  mapStringLayer["GuardPath1"] = GUARD_PATH_1;
  mapStringLayer["GuardPath2"] = GUARD_PATH_2;
  mapStringLayer["GuardPath3"] = GUARD_PATH_3;
  mapStringLayer["GuardPath4"] = GUARD_PATH_4;
  mapStringLayer["GuardPath5"] = GUARD_PATH_5;
  mapStringLayer["HealthPickups"] = HEALTH_PICKUPS;
  mapStringLayer["WeaponPickups"] = WEAPON_PICKUPS;
  mapStringLayer["KeyPickups"] = KEY_PICKUPS;
  mapStringLayer["EndPoint"] = END_POINT;
  mapStringLayer["EndLevelItem"] = END_LEVEL_ITEM;
  mapStringLayer["Bushes"] = BUSHES;
  mapStringLayer["Terminals"] = TERMINALS;
  mapStringLayer["Cameras"] = CAMERAS;
  mapStringLayer["Alarms"] = ALARMS;
  mapStringLayer["Tripwires"] = TRIPWIRES;
  mapStringLayer["BreakableWallsH"] = BREAKABLE_WALLS_H;
  mapStringLayer["BreakableWallsV"] = BREAKABLE_WALLS_V;
  mapStringLayer["OpendDoorsH"] = OPEN_DOORS_H;
  mapStringLayer["OpendDoorsV"] = OPEN_DOORS_V;
  mapStringLayer["HackDoorsH"] = HACK_DOORS_H;
  mapStringLayer["HackDoorsV"] = HACK_DOORS_V;
  mapStringLayer["LockedDoorsH"] = LOCKED_DOORS_H;
  mapStringLayer["LockedDoorsV"] = LOCKED_DOORS_V;
  mapStringLayer["Background"] = BACKGROUND;
  mapStringLayer["MapBorders"] = MAP_BORDERS;
  mapStringLayer["Walls"] = FLOOR;
  mapStringLayer["Walls"] = WALLS;
  mapStringLayer["Walls"] = WINDOWS;
}*/
