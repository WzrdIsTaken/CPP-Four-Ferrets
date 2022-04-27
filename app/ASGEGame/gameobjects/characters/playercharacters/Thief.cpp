// Created by Ben on 10/03/2022

#include "../../mapstuff/IPickable.hpp"

#include "Thief.hpp"

void Thief::start()
{
  // Attributes
  speed             = 150.0F;
  ability_cool_down = 10.0;
  character_class   = PlayerCharacter::CharacterClasses::THIEF;

  // Init base class
  PlayerCharacter::start();

  // Sprite / Animator
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/images/thief/thief_idle.png", { 480.0F, 270.0F }, 1.0F);
  sprite->setGlobalZOrder(10);

  std::map<std::string, std::vector<std::string>> animations = {
    { IDLE_ANIMATION, { "/data/images/thief/thief_idle.png" } },
    { RUN_ANIMATION,
      { "/data/images/thief/thief_run_1.png", "/data/images/thief/thief_run_2.png" } },
    { DEATH_ANIMATION, { "/data/images/thief/thief_dead.png" } }
  };

  animator = std::make_unique<bcw::Animator>(sprite.get(), animations, 0.12F);
  animator->setAnimation(IDLE_ANIMATION);

  // Just thief things B)
  NORMAL_SPEED = speed;
}

void Thief::update(const ASGE::GameTime& game_time)
{
  if (ability_duration_clock > 0)
  {
    ability_duration_clock -= game_time.deltaInSecs();

    if (ability_duration_clock <= 0)
    {
      speed = NORMAL_SPEED;
    }
  }

  PlayerCharacter::update(game_time);
}

void Thief::render(const ASGE::GameTime& game_time)
{
  PlayerCharacter::render(game_time);
}

void Thief::ability(const bcw::Vector2& use_direction)
{
  if (abilityReady())
  {
    ability_duration_clock = ABILITY_DURATION;
    speed                  = 200.0F;

    CharacterBase::ability(use_direction);
  }
}

void Thief::interact()
{
  std::vector<bcw::Collider*> collisions =
    bcw::Driver::getCollisionHandler()->checkCollisionsInArea(
      sprite->xPos(), sprite->yPos(), interaction_range, interaction_range);
  int potential_picks = 0;

  for (const auto& collision : collisions)
  {
    std::shared_ptr<IPickable> pickable =
      std::dynamic_pointer_cast<IPickable>(collision->getAttachedScene());

    if (pickable != nullptr)
    {
      if (pickable->getPickedState())
      {
        potential_picks++;
      }

      pickable->onPickBegin(PICK_DURATION);
    }
  }

  if (potential_picks == 0)
  {
    CharacterBase::interact();
  }

  // pick locks
}
