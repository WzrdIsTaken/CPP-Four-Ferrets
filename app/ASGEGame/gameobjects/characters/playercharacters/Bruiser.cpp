// Created by Ben on 31/03/2022

#include "Bruiser.hpp"

void Bruiser::start()
{
  // Attributes
  speed             = 100.0F;
  health            = 150;
  ability_cool_down = 15.0;
  character_class   = PlayerCharacter::CharacterClasses::BRUISER;

  // Init base class
  PlayerCharacter::start();

  // Sprite / Animator
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/images/bruiser/bruiser_idle.png", { 100, 100 }, 1.0F);
  sprite->setGlobalZOrder(10);

  std::map<std::string, std::vector<std::string>> animations = {
    { IDLE_ANIMATION, { "/data/images/bruiser/bruiser_idle.png" } },
    { RUN_ANIMATION,
      { "/data/images/bruiser/bruiser_run_1.png", "/data/images/bruiser/bruiser_run_2.png" } },
    { DEATH_ANIMATION, { "/data/images/bruiser/bruiser_dead.png" } }
  };

  animator = std::make_unique<bcw::Animator>(sprite.get(), animations, 0.12F);
  animator->setAnimation(IDLE_ANIMATION);
}

void Bruiser::update(const ASGE::GameTime& game_time)
{
  PlayerCharacter::update(game_time);
}

void Bruiser::render(const ASGE::GameTime& game_time)
{
  PlayerCharacter::render(game_time);
}

void Bruiser::ability(const bcw::Vector2& use_direction)
{
  if (abilityReady())
  {
    // knock out guard
    // r a y c a s t
    // or maybe not
    // should be like a slam

    std::vector<bcw::Collider*> collisions =
      bcw::Driver::getCollisionHandler()->checkCollisionsInArea(
        sprite->xPos(), sprite->yPos(), SLAM_AREA.x, SLAM_AREA.y);

    for (const auto& collision : collisions)
    {
      if (collision->getAttachedScene() == nullptr)
      {
        continue;
      }

      if (collision->getAttachedScene()->getTag() == "NPC")
      {
        std::dynamic_pointer_cast<CharacterBase>(collision->getAttachedScene())->knockOut(STUN_TIME);
      }
    }

    bcw::Driver::getAudioHandler()->playAudio("/data/audio/game_sfx/BIG_punch_sound.mp3");

    CharacterBase::ability(use_direction);
  }
}

void Bruiser::interact()
{
  CharacterBase::interact();
}
