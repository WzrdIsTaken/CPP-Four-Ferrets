// Created by Ben on 12/04/2022

// For bcw::MoreMaths (rad2Degrees)
#include <ASGEGameLib/HelperMethods.hpp>

// Header
#include "Guard.hpp"

void Guard::start()
{
  // Attributes
  speed  = 100.0F;
  health = 30;

  // Init base class
  NonPlayerCharacter::start();

  // Sprite
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/images/guard/guard_idle.png", { 100, 100 }, 1.0F);

  // Animator
  std::map<std::string, std::vector<std::string>> animations = {
    { IDLE_ANIMATION, { "/data/images/guard/guard_idle.png" } },
    { RUN_ANIMATION,
      { "/data/images/guard/guard_run_1.png", "/data/images/guard/guard_run_2.png" } },
    { DEATH_ANIMATION, { "/data/images/guard/guard_dead.png" } }
  };

  animator = std::make_unique<bcw::Animator>(sprite.get(), animations, 0.12F);
  animator->setAnimation(IDLE_ANIMATION);

  // Alarm Manager
  // alarm_manager = bcw::Driver::getSceneHandler()->findScene<AlarmManager>();
}

void Guard::update(const ASGE::GameTime& game_time)
{
  // Tick base class
  NonPlayerCharacter::update(game_time);

  // If the guard isn't dead, switch through the possible npc states: default, chasing and fleeing
  if (!getIsDead())
  {
    // Decrement the guards attack timer, if it's less than 0 the guard can attack
    attack_timer > 0.0 ? attack_timer -= game_time.deltaInSecs() : attack_timer = 0.0;

    if(is_attacking && target != nullptr)
    {
      attack();
    }

    castRayAndCheckForPlayer(false);
    // Switch over the possible npc states: Default, Chasing and Fleeing
    switch (npc_state)
    {
      // Default state (the guard is just patrolling), detect player via a LoS raycast
      case NpcState::DEFAULT:
      {
        // Check if a player has been spotted
        if (castRayAndCheckForPlayer(false) && !attempting_lock_on)
        {
          // If they have, set the lock on timer
          lock_on_timer      = LOCK_ON_TIME;
          attempting_lock_on = true;

          // And display the questioning ('?') sprite
          exclamation_sprite->loadTexture("/data/images/guard/questioning.png");
          exclamation_sprite->opacity(1.0F);
        }

        // If a player has been spotted, tick down the lock on timer
        if (attempting_lock_on)
        {
          countDownToLockOn(game_time.deltaInSecs());
        }
        break;
      }

      // If the guard is chasing the player, check they are in range with a distance check
      case NpcState::CHASING:
      {
        // Check if the distance is not larger than CHASE_RANGE and that the target is visible
        if ((!checkIfTargetIsInRange() || !target->getIsVisible()) && !attempting_lock_off)
        {
          // Load the questioning ('?') sprite if the player is no longer visible (e.g., hacker
          // cloak) or the frustrated ('...') sprite if the player has got out of range
          exclamation_sprite->loadTexture(
            !target->getIsVisible() ? "/data/images/guard/questioning.png"
                                    : "/data/images/guard/frustrated.png");
          exclamation_sprite->opacity(1.0F);

          // Count down to locking off
          lock_off_timer      = LOCK_OFF_TIME;
          attempting_lock_off = true;
        }

        // If the player is no longer in range, tick down the lock off timer
        if (attempting_lock_off)
        {
          countDownToLockOff(game_time.deltaInSecs());
        }
        break;
      }

      // If the guard is fleeing, do something soontm? [but not a priority at all]
      case NpcState::FLEEING:
        // In the current iteration of the game, guards cannot flee
        break;
    }
  }
}

void Guard::setAlarmManager(const std::shared_ptr<AlarmManager>& _alarm_manger)
{
  alarm_manager = _alarm_manger;
}

void Guard::onCollisionEnter(bcw::Collider* collision)
{
  // If the guard collides with the player, attack!
  if (collision->getAttachedScene()->getTag() == "Player" && target == collision->getAttachedScene())
  {
    is_attacking = true;
  }
}

void Guard::onCollisionExit(bcw::Collider* collision)
{
  if (target == collision->getAttachedScene())
  {
    is_attacking = false;
  }
}

void Guard::countDownToLockOn(const double delta_time)
{
  // Decrement the timer by delta time every frame
  lock_on_timer -= delta_time;

  // If the timer reaches zero
  if (lock_on_timer < 0.0)
  {
    attempting_lock_on = false;

    // Check that the player is still in sight, if so
    if (castRayAndCheckForPlayer(true))
    {
      // Sound the alarm
      alarm_manager->activate(ALARM_DURATION, AlarmManager::DurationMode::ADDITION);

      // Set give the guard some speed increase
      speed = DEFAULT_SPEED * CHASING_SPEED_MODIFIER;

      // And display the exclamation ('!') sprite
      exclamation_sprite->loadTexture("/data/images/guard/exclamation.png");
      exclamation_sprite->opacity(1.0F);
    }
    // Otherwise, if the player wasn't spotted disable the questioning ('?') sprite
    else
    {
      exclamation_sprite->opacity(0.0F);
    }
  }
}

void Guard::countDownToLockOff(const double delta_time)
{
  // Decrement the timer by delta time every frame
  lock_off_timer -= delta_time;

  // If the timer is 0, check that the target is still out of range or not visible
  if (lock_off_timer < 0.0)
  {
    // Regardless if the lock off was a success or not, the timer needs to be reset
    attempting_lock_off = false;
    lock_off_timer      = LOCK_OFF_TIME;

    // If the player is still out of range, reset the guards state
    if (!checkIfTargetIsInRange() || !target->getIsVisible())
    {
      // If the guard is dead, then the state and variables are already set
      if (!getIsDead())
      {
        changeNpcState(NonPlayerCharacter::NpcState::DEFAULT, nullptr);
        exclamation_sprite->opacity(0.0F);
        speed = DEFAULT_SPEED * NORMAL_SPEED_MODIFIER;

        // Todo: Once the pathfinder is in properly I am not sure this will be needed
        velocity.x = 0;
        velocity.y = 0;
      }
    }
    // Otherwise, toggle back on the exclamation ('!') sprite and keep chasing!
    else
    {
      exclamation_sprite->loadTexture("/data/images/guard/exclamation.png");
    }
  }
}

bool Guard::castRayAndCheckForPlayer(const bool set_locked_onto_player)
{
  /* Many bot attempts here...
   *
  //float rot = sprite->rotationInRadians();
  //bcw::Vector2 direction(cos(rot), sin(rot)); // This should work?

  // Bot solution
  // I honestly can't work out why cos(rot), sin(rot) isn't working so as the guards
  // can only move in four directions anyway imma just go with this
  // https://math.stackexchange.com/questions/180874/convert-angle-radians-to-a-heading-vector

  float rot_as_degrees = bcw::MoreMath::rad2Degrees(rot);
   if (rot_as_degrees < 90.0F) // NOLINT
    direction = { -1, 0 };
  else if (rot_as_degrees < 180.0F) // NOLINT
    direction = { 0, -1 };
  else if (rot_as_degrees < 270.0F) // NOLINT
    direction = { 1, 0 };
  else // NOLINT
    direction = { 0, 1 };
  */
  auto norm_vec = velocity;
  // Cast the ray
  bcw::HitResult result = raycaster->castRay({ sprite->xPos(), sprite->yPos() }, norm_vec.normalise(), VIEW_RANGE);

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

    // Otherwise, if these flags are true, set the guard to the new state
    if (set_locked_onto_player && !getIsDead())
    {
      changeNpcState(NonPlayerCharacter::NpcState::CHASING, potential_lock_on);
    }

    // Return true if the ray hit a player
    return true;
  }

  // Return false if the ray did not hit a player
  return false;
}

bool Guard::checkIfTargetIsInRange()
{
  // Get the distance between the guard and the player
  bcw::Vector2 current_position = { sprite->xPos(), sprite->yPos() };
  bcw::Vector2 target_position  = { target->getSprite()->xPos(), target->getSprite()->yPos() };
  float distance_to_target      = current_position.distance(target_position);

  // Return if the distance is less than CHASE_RANGE
  return distance_to_target < CHASE_RANGE && !target->getIsDead();
}

void Guard::attack()
{
  if (npc_state == NonPlayerCharacter::NpcState::CHASING && attack_timer <= 0.0 && !isKnockedOut())
  {
    bcw::Driver::getAudioHandler()->playAudio("data/audio/punch.mp3");
    target->takeDamage(DAMAGE);
    attack_timer = ATTACK_COOL_DOWN;
  }
  if (target->getIsDead() || target == nullptr)
  {
    is_attacking = false;
  }
}
