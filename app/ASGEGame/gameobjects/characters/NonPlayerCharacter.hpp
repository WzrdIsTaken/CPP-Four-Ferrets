// Created by Ben on 10/03/2022

#pragma once

// Components
#include <ASGEGameLib/components/collision/Raycast.hpp>
//#include <ASGEGameLib/components/tilemap/PathFinder.hpp>

// New pathfinder logic
#include "../mapstuff/MapManager.hpp"

// Base class
#include "CharacterBase.hpp"

// The base class which all non player characters (e.g. guards, civilians) inherit from

class NonPlayerCharacter : public CharacterBase
{
 public:
  NonPlayerCharacter()           = default;
  ~NonPlayerCharacter() override = default;

  // Initialize NonPlayerCharacter variables
  void start() override;
  // Tick the NPC's state based on NpcState
  void update(const ASGE::GameTime& game_time) override;
  // Render the exclamation_sprite
  void render(const ASGE::GameTime& game_time) override;

  // Set the NPC's initial position and pass in the pathfinder reference
  void init(const bcw::Vector2& spawn_position, MapManager* _path_finder);

  // Which way the NPC will move through their patrol points
  enum class PatrolDirection
  {
    FORWARD,
    BACKWARD
  };
  // Set the NPC's patrol route and the initial direction they will move through it
  void setPatrolRoute(
    const std::vector<bcw::Vector2>& _patrol_route,
    PatrolDirection start_patrol_direction = PatrolDirection::FORWARD);

  // The possible states of the NPC
  enum class NpcState
  {
    DEFAULT, // Moving through the patrol route
    CHASING, // Moving towards the target player
    FLEEING  // Running away from the player [note - currently not implemented]
  };
  // Pass in nullptr for the target is new_state is DEFAULT ty xx
  void changeNpcState(NpcState new_state, const std::shared_ptr<CharacterBase>& new_target);

  // Makes the npc move faster when an alarm is raised
  void alarmsActivated(bool activated);

 protected:
  // Disables the exclamation sprite
  void onDie() override;

  // Components
  std::unique_ptr<bcw::Raycast> raycaster = nullptr;

  // The sprite showing the NPC's emotion ('!', '?' or '...')
  std::unique_ptr<ASGE::Sprite> exclamation_sprite = nullptr;
  const bcw::Vector2 EXCLAMATION_SPRITE_OFFSET     = { -7.5F, 20.0F };

  // The player the npc will chase or flee from
  std::shared_ptr<CharacterBase> target;

  // The NPC's current state
  NpcState npc_state = NpcState::DEFAULT;

  // The NPC's current path between patrol points
  std::deque<bcw::Vector2> path;

  // Attributes
  double idle_time = 1.0; // Time between reaching a pathfinding destination and moving on

  // Modifiers
  float DEFAULT_SPEED               = 0.0F; // Would be a const if it could. Don't change this!
  const float NORMAL_SPEED_MODIFIER = 0.75F;
  const float ALARM_RAISED_SPEED_MODIFIER = 1.0F;
  const float CHASING_SPEED_MODIFIER      = 1.10F;

 private:
  // Moves the NPC down the path
  void handlePathFinding(double delta_time);
  // Calculates a path based off the NPC's state
  void calculatePath();
  // Prevents code repetition...
  void calculatePatrolRoutePath(PatrolDirection _patrol_direction);

  // The pathfinder reference
  MapManager* path_finder = nullptr; // can be made const*?

  // The predefined path the npc will follow if its set
  std::vector<bcw::Vector2> patrol_route;
  // Which way the NPC will move through the path
  PatrolDirection patrol_direction = PatrolDirection::FORWARD;

  // How close the npc has to get to the target path destination before it moves to the next one
  const float SENSITIVITY = 5.0F;
  // A timer counting how long the NPC has been waiting between patrol points
  double idle_time_timer = 0.0;
  bool idling            = false;

  // A very bot fix
  bcw::Vector2 rotation{};
};
