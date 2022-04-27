// Created by Ben on 12/04/2022

// For bcw::MoreMaths (degrees2Rad, rad2Degrees and lookAt)
#include <ASGEGameLib/HelperMethods.hpp>

// Header
#include "SecurityCamera.hpp"

void SecurityCamera::init(
  const bcw::Vector2& position, const float start_rotation, const float _sweep_near,
  const float _sweep_far, const float _sweep_speed,
  const std::shared_ptr<AlarmManager>& _alarm_manger) // NOLINT
{
  // Sprite
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "data/images/camera/security_camera_idle_1.png", position, 1.0F);
  sprite->rotationInRadians(bcw::MoreMath::degrees2Rad(start_rotation));
  current_angle = sprite->rotationInRadians();

  // Animator
  std::map<std::string, std::vector<std::string>> animations = {
    { "idle",
      { "data/images/camera/security_camera_idle_1.png",
        "data/images/camera/security_camera_idle_2.png" } },
    { "active",
      { "data/images/camera/security_camera_active_1.png",
        "data/images/camera/security_camera_active_2.png" } },
    { "disabled", { "data/images/camera/security_camera_disabled.png" } }
  };

  animator = std::make_unique<bcw::Animator>(sprite.get(), animations, 0.75F);
  animator->setAnimation("idle");

  // Initial values
  sweep_near  = _sweep_near;
  sweep_far   = _sweep_far;
  sweep_speed = _sweep_speed;

  // Raycast!
  raycaster = std::make_unique<bcw::Raycast>();

  // Watch out
  alarm_manager = _alarm_manger;
}

void SecurityCamera::update(const ASGE::GameTime& game_time)
{
  // Tick the animator
  animator->update(game_time);

  // Switch through the camera states: Sweep, Locked and Disabled
  switch (camera_state)
  {
    // The basic state for the camera - looking for a player to lock on to
    case CameraState::SWEEP:
    {
      // Move camera from side to side
      float sweep_step = sweep_speed * static_cast<float>(game_time.deltaInSecs());
      current_angle    = current_angle + sweep_direction * sweep_step;
      sprite->rotationInRadians(current_angle);

      float current_angle_degrees = bcw::MoreMath::rad2Degrees(sprite->rotationInRadians());
      if (current_angle_degrees > sweep_far || current_angle_degrees < sweep_near)
      {
        sweep_direction *= -1;
      }

      // Check if a player has been spotted
      if (castRayAndCheckForPlayer(false) && !attempting_lock_on)
      {
        lock_on_timer      = LOCK_ON_TIME;
        attempting_lock_on = true;
      }

      // If a player has been spotted, tick down the lock on timer
      if (attempting_lock_on)
      {
        countDownToLockOn(game_time.deltaInSecs());
      }
      break;
    }

    // When a player has been spotted
    case CameraState::LOCKED:
    {
      // Get the character who we have locked onto sprites and work out the angle towards it
      ASGE::Sprite* character_sprite = locked_onto_character->getSprite();
      float desired_rotation         = bcw::MoreMath::lookAt(
        { sprite->xPos(), sprite->yPos() }, { character_sprite->xPos(), character_sprite->yPos() });

      // Check if the desired rotation is within the bounds of the cameras sweep
      // and rotate the camera
      float desired_rot_degrees = bcw::MoreMath::rad2Degrees(desired_rotation);
      if (desired_rot_degrees > sweep_far)
      {
        desired_rotation = bcw::MoreMath::degrees2Rad(sweep_far);
      }
      if (desired_rot_degrees < sweep_near)
      {
        desired_rotation = bcw::MoreMath::degrees2Rad(sweep_near);
      }
      sprite->rotationInRadians(desired_rotation);

      // Cast a ray, and if it does not collide with the player then attempt to lock off
      if (!castRayAndCheckForPlayer(false) && !attempting_lock_off)
      {
        lock_off_timer      = LOCK_OFF_TIME;
        attempting_lock_off = true;
      }

      // If the player is no longer in line of sight, tick down the lock off timer
      if (attempting_lock_off)
      {
        countDownToLockOff(game_time.deltaInSecs());
      }
      break;
    }

    // When the camera has been hacked via a computer terminal
    case CameraState::DISABLED:
      // If the camera is disabled, do nothing
      break;
  }
}

void SecurityCamera::countDownToLockOn(const double delta_time)
{
  // Decrement the timer by delta time every frame
  lock_on_timer -= delta_time;

  // Once the timer reaches 0, fire another ray and check if the player is still visible
  if (lock_on_timer < 0.0)
  {
    attempting_lock_on = false;

    // If they are visible, set off the alarms
    if (castRayAndCheckForPlayer(true))
    {
      alarm_manager->activate(ALARM_DURATION, AlarmManager::DurationMode::ADDITION);
      bcw::Driver::getAudioHandler()->playAudio("data/audio/game_sfx/camera_trigger.mp3", 0.2F);
    }
  }
}

void SecurityCamera::countDownToLockOff(double delta_time)
{
  // Decrement the timer by delta time every frame
  lock_off_timer -= delta_time;

  // If the timer is 0, fire off another ray and check if the target is in line of sight
  if (lock_off_timer < 0.0)
  {
    // Regardless if the lock off was a success or not, the timer needs to be reset
    attempting_lock_off = false;
    lock_off_timer      = LOCK_OFF_TIME;

    // If the ray doesn't hit the player, lock off and rest the camera
    if (!castRayAndCheckForPlayer(false))
    {
      locked_onto_character = nullptr;
      current_angle         = sprite->rotationInRadians();

      // If the camera is disabled, then the state and variables are already set
      if (camera_state != CameraState::DISABLED)
      {
        camera_state = CameraState::SWEEP;
        animator->setAnimation("idle");
      }
    }
  }
}

bool SecurityCamera::castRayAndCheckForPlayer(const bool set_locked_onto_player)
{
  // Set the raycast direction to the direction the camera is facing
  float rot = sprite->rotationInRadians();
  bcw::Vector2 direction(cos(rot), sin(rot));

  // Cast the ray
  bcw::HitResult result =
    raycaster->castRay({ sprite->xPos() + 1, sprite->yPos() + 1 }, direction, CAMERA_RANGE);

  // If the ray hits the player...
  if (result.hit != nullptr && result.hit->getAttachedScene()->getTag() == "Player")
  {
    // Get the CharacterBase component of the player
    std::shared_ptr<CharacterBase> potential_lock_on =
      std::dynamic_pointer_cast<CharacterBase>(result.hit->getAttachedScene());

    // If the player is invisible (e.g. the hacker cloaked) return false - no lock on
    if (!potential_lock_on->getIsVisible() || potential_lock_on->getIsDead())
    {
      return false;
    }

    // Otherwise, if these flags are true, set the camera to the next state
    if (set_locked_onto_player && camera_state != CameraState::DISABLED)
    {
      camera_state          = CameraState::LOCKED;
      locked_onto_character = potential_lock_on;
      animator->setAnimation("active");
    }

    // Return true if the ray hit a player
    return true;
  }

  // Return false if the ray did not hit a player
  return false;
}

void SecurityCamera::onHackBegin(float /*hack_speed*/)
{
  // In this game, once a camera is disabled it can never be enabled again so just set the state
  camera_state          = CameraState::DISABLED;
  locked_onto_character = nullptr;
  animator->setAnimation("disabled");
}

void SecurityCamera::onHackEnd() {}

bool SecurityCamera::getHackedState()
{
  // A hacked camera will always be disabled
  return camera_state == CameraState::DISABLED;
}
