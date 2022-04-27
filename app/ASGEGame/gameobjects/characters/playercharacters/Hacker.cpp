// Created by Ben on 10/03/2022

#include "../../mapstuff/IHackable.hpp"

#include "Hacker.hpp"

void Hacker::start()
{
  // Attributes
  speed             = 125.0F;
  ability_cool_down = 15.0;
  character_class   = PlayerCharacter::CharacterClasses::HACKER;

  // Init base class
  PlayerCharacter::start();

  // Sprite / Animator
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/images/hacker/hacker_idle.png", { 100, 100 }, 1.0F);
  sprite->setGlobalZOrder(10);

  std::map<std::string, std::vector<std::string>> animations = {
    { IDLE_ANIMATION, { "/data/images/hacker/hacker_idle.png" } },
    { RUN_ANIMATION,
      { "/data/images/hacker/hacker_run_1.png", "/data/images/bruiser/bruiser_run_2.png" } },
    { DEATH_ANIMATION, { "/data/images/hacker/hacker_dead.png" } }
  };

  animator = std::make_unique<bcw::Animator>(sprite.get(), animations, 0.12F);
  animator->setAnimation(IDLE_ANIMATION);
}

void Hacker::update(const ASGE::GameTime& game_time)
{
  if (ability_duration_clock > 0)
  {
    ability_duration_clock -= game_time.deltaInSecs();

    if (ability_duration_clock <= 0)
    {
      is_visible = true;
      makeHackerAndHeldItemsInvisible(NORMAL_ALPHA_VALUE);
    }
  }

  PlayerCharacter::update(game_time);
}

void Hacker::render(const ASGE::GameTime& game_time)
{
  PlayerCharacter::render(game_time);
}

void Hacker::ability(const bcw::Vector2& use_direction)
{
  if (abilityReady())
  {
    // show all enemy positions
    // would only work though if we had lights
    // or
    // i n v i s i b i l i t y ???aa

    is_visible             = false;
    ability_duration_clock = ABILITY_DURATION;
    makeHackerAndHeldItemsInvisible(INVISIBLE_ALPHA_VALUE);

    CharacterBase::ability(use_direction);
  }
}

void Hacker::interact()
{
  std::vector<bcw::Collider*> collisions =
    bcw::Driver::getCollisionHandler()->checkCollisionsInArea(
      sprite->xPos(), sprite->yPos(), interaction_range, interaction_range);
  int potential_hacks = 0;

  for (const auto& collision : collisions)
  {
    std::shared_ptr<IHackable> hackable =
      std::dynamic_pointer_cast<IHackable>(collision->getAttachedScene());

    if (hackable != nullptr)
    {
      if (!hackable->getHackedState())
      {
        potential_hacks++;
      }
      hackable->onHackBegin(HACK_DURATION);
    }
  }

  if (potential_hacks == 0)
  {
    CharacterBase::interact();
  }

  // hack pc terminals
}

void Hacker::makeHackerAndHeldItemsInvisible(const float invisibility_value)
{
  sprite->opacity(invisibility_value);

  for (const auto& item : getInventory())
  {
    item->getSprite()->opacity(invisibility_value);
  }
}
