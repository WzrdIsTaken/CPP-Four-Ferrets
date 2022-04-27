//
// Created by Norbert on 15/04/2022.
//

#ifndef ASGEGAME_MAPMANAGER_HPP
#define ASGEGAME_MAPMANAGER_HPP

#include "ASGEGameLib/Component.hpp"
#include "ASGEGameLib/components/map/TileSquare.hpp"
#include "ASGEGameLib/components/tilemap/PathFinder.hpp"
#include <map>
#include <tmxlite/Map.hpp>
#include <vector>

//#include "gameobjects/mapstuff/collectables/CollectableBase.hpp"

class MapManager : public bcw::Component
{
 public:
  MapManager(int magnification, std::string level);
  ~MapManager() override = default;
  void initMap();
  bool isPassable(unsigned int index);
  [[nodiscard]] float getTileSize() const;
  std::vector<std::unique_ptr<TileSquare>>& getTiles();
  ASGE::Point2D checkCollision(
    const ASGE::Sprite& sprite, float radius, float speed, ASGE::Point2D vector,
    const ASGE::GameTime& game_time);
  void renderMap();
  ASGE::Point2D getPlayerStartPos(uint16_t idx);
  ASGE::Point2D getMapSize();
  std::deque<bcw::Vector2> findPath(ASGE::Point2D start, ASGE::Point2D destination);

 private:
  void buildTiles(tmx::Layer& layer, int16_t z);
  void buildObjects(tmx::Layer& layer);
  uint16_t getTileSet(uint32_t tile_id);
  float getDistance(ASGE::SpriteBounds obj1, ASGE::SpriteBounds obj2);
  ASGE::SpriteBounds
  predictBounds(ASGE::SpriteBounds curr_bound, float speed_dt, ASGE::Point2D vector);
  void addBlockToPathfinder(int x, int y);
  void initHackableDoors();

  std::unique_ptr<bcw::PathFinder> path_finder;
  const std::string DEFAULT_MAP_FOLDER{ "data/map/" };
  std::string map_name = "testMap.tmx";
  int magnification    = 1;
  int tile_size        = 32;
  ASGE::FILEIO::File tile_map;
  ASGE::FILEIO::IOBuffer buffer;
  tmx::Map map;
  std::vector<std::unique_ptr<TileSquare>> tiles;
  std::vector<TileSquare*> collision_tiles;
  std::vector<ASGE::Sprite*> collision_doors;

  ASGE::Point2D map_size{ 0.0F, 0.0F };
  std::vector<ASGE::Point2D> player_starting_locations;
  std::vector<uint32_t> tileset_GIDs;
  std::vector<std::pair<std::shared_ptr<bcw::Scene>, bcw::Vector2>> hack_doors;
  std::vector<std::pair<std::shared_ptr<bcw::Scene>, bcw::Vector2>> hack_door_terminals;

  std::shared_ptr<bcw::Scene> alarm_manager = nullptr;
  std::vector<bcw::Vector2> guard_one_path{};
  std::vector<bcw::Vector2> guard_two_path{};
  std::vector<bcw::Vector2> guard_three_path{};
  std::vector<bcw::Vector2> guard_four_path{};
  std::vector<bcw::Vector2> guard_five_path{};
  /*enum Layer
  {
    PLAYER_SPAWN_POSITIONS,
    GUARD_PATH_1,
    GUARD_PATH_2,
    GUARD_PATH_3,
    GUARD_PATH_4,
    GUARD_PATH_5,
    HEALTH_PICKUPS,
    WEAPON_PICKUPS,
    KEY_PICKUPS,
    END_POINT,
    END_LEVEL_ITEM,
    BUSHES,
    TERMINALS,
    CAMERAS,
    ALARMS,
    TRIPWIRES,
    BREAKABLE_WALLS_H,
    BREAKABLE_WALLS_V,
    OPEN_DOORS_H,
    OPEN_DOORS_V,
    HACK_DOORS_H,
    HACK_DOORS_V,
    LOCKED_DOORS_H,
    LOCKED_DOORS_V,
    BACKGROUND,
    MAP_BORDERS,
    FLOOR,
    WALLS,
    WINDOWS
  };
  static std::map<std::string, Layer> mapStringLayer;
  char smtg[h];*/
};

#endif // ASGEGAME_MAPMANAGER_HPP
