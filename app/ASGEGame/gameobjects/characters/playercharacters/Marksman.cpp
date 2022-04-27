// Created by Ben on 31/03/2022

#include "../../projectiles/KnockOutDart.hpp"

#include "Marksman.hpp"

void Marksman::start()
{
  // Attributes
  speed             = 125.0F;
  health            = 75;
  ability_cool_down = 12.5;
  character_class   = PlayerCharacter::CharacterClasses::MARKSMAN;

  // Init base class
  PlayerCharacter::start();

  // Sprite / Animator
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/images/marksman/marksman_idle.png", { 100, 100 });
  sprite->setGlobalZOrder(10);

  std::map<std::string, std::vector<std::string>> animations = {
    { IDLE_ANIMATION, { "/data/images/marksman/marksman_idle.png" } },
    { RUN_ANIMATION,
      { "/data/images/marksman/marksman_run_1.png", "/data/images/marksman/marksman_run_2.png" } },
    { DEATH_ANIMATION, { "/data/images/marksman/marksman_dead.png" } }
  };

  animator = std::make_unique<bcw::Animator>(sprite.get(), animations, 0.12F);
  animator->setAnimation(IDLE_ANIMATION);
}

void Marksman::update(const ASGE::GameTime& game_time)
{
  PlayerCharacter::update(game_time);
}

void Marksman::render(const ASGE::GameTime& game_time)
{
  PlayerCharacter::render(game_time);
}

void Marksman::ability(const bcw::Vector2& use_direction)
{
  if (abilityReady())
  {
    // shoot knockout dart
    std::shared_ptr<KnockOutDart> knock_out_dart =
      bcw::Driver::getSceneHandler()->addGameObject<KnockOutDart>();
    knock_out_dart->init(
      ProjectileBase::Target::NPC, { sprite->xPos(), sprite->yPos() }, use_direction, 0);
    // knock_out_dart->init(ProjectileBase::Target::NPC, sprite->rotationInRadians(), 0);

    CharacterBase::ability(use_direction);
  }
}

void Marksman::interact()
{
  CharacterBase::interact();
}
