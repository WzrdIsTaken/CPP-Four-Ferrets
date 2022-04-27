// Created by Ben on 12/04/2022

#pragma once

// Components
#include <ASGEGameLib/components/animator/Animator.hpp>
#include <ASGEGameLib/components/collision/Raycast.hpp>

// References
#include "../../characters/CharacterBase.hpp"
#include "../alarm/AlarmManager.hpp"

// Base class(s)
#include "../IHackable.hpp"
#include "../MapObject.hpp"

// A basic security camera which detects a target via raycasting and sets off the alarms
// if a player is spotted

class SecurityCamera
  : public MapObject,
    public IHackable // IHackable but no collider = linked up from a ComputerTerminal
{
 public:
  SecurityCamera()           = default;
  ~SecurityCamera() override = default;

  // Init the camera
  // position - The location of the camera
  // start_rotation - The cameras initial rotation. Should be between _sweep_near and _sweep_far
  // _sweep_near, _sweep_far - How far the camera will rotate to either direction
  // _sweep_speed - How fast the camera will rotate
  void init(
    const bcw::Vector2& position, float start_rotation, float _sweep_near, float _sweep_far,
    float _sweep_speed, const std::shared_ptr<AlarmManager>& _alarm_manger);
  // Tick the camera based off CameraState
  void update(const ASGE::GameTime& game_time) override;

 private:
  // Once a player has been spotted, count down from LOCK_ON_TIME for lock on
  void countDownToLockOn(double delta_time);
  // Once the target is out of line of sight, count down from LOCK_OFF_TIME for lock off
  void countDownToLockOff(double delta_time);
  // Check if a player or the target is in line of sight
  bool castRayAndCheckForPlayer(bool set_locked_onto_player);

  // IHackable methods. In this game cameras are hacked from computer terminals,
  // and once they are disabled are not turned back on
  void onHackBegin(float hack_speed) override;
  void onHackEnd() override;
  bool getHackedState() override;

  // Components
  std::unique_ptr<bcw::Raycast> raycaster = nullptr;
  std::unique_ptr<bcw::Animator> animator = nullptr;

  // Contains a reference to all the alarms in the level, so they can be set off
  std::shared_ptr<AlarmManager> alarm_manager = nullptr;
  const float ALARM_DURATION                  = 10.0F; // How long the alarms will be set off for

  // The player the camera will track once locked on
  std::shared_ptr<CharacterBase> locked_onto_character = nullptr;

  // The possible states of the camera
  enum class CameraState
  {
    SWEEP,   // Default, moving from side to side looking for a target
    LOCKED,  // Locked onto a player and tracking their movements
    DISABLED // Hacked
  };
  CameraState camera_state = CameraState::SWEEP;

  // The cameras' behaviour changing variables
  float sweep_near         = 0.0F;
  float sweep_far          = 0.0F;
  float sweep_speed        = 0.0F;
  float current_angle      = 0.0F;
  float sweep_direction    = 1.0F;
  const float CAMERA_RANGE = 2000.0F;

  const double LOCK_ON_TIME = 0.1; // How long the camera will take to lock onto a player
  double lock_on_timer      = 0.0;
  bool attempting_lock_on   = false;

  const double LOCK_OFF_TIME = 1.5;   // How long the camera will take to lock off a player
  double lock_off_timer      = 0.0;   // doubling up for verboseness
  bool attempting_lock_off   = false; // ^
};
