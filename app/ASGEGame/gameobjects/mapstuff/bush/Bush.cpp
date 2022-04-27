// Created by Ben on 12/04/2022

#include "../../characters/CharacterBase.hpp"

#include "Bush.hpp"

void Bush::start()
{
  // Init base class
  MapObject::start();

  // Sprite
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/images/bush/Bush.png", { 0, 0 }, 1.F);
  sprite->opacity(1.F);

  // Collision
  collider = std::make_unique<bcw::Collider>(
    [this](bcw::Collider* collision) { onCollisionEnter(collision); },
    [this](bcw::Collider* collision) { onCollisionExit(collision); },
    sprite.get(),
    this->shared_from_this());
  collider->setRaycastTarget(false);
}

void Bush::init(const bcw::Vector2& position)
{
  sprite->xPos(position.x);
  sprite->yPos(position.y);
}

void Bush::onCollisionEnter(bcw::Collider* collision)
{
  if (collision->getAttachedScene()->getTag() == "Player")
  {
    std::dynamic_pointer_cast<CharacterBase>(collision->getAttachedScene())->setIsVisible(false);
    bcw::Driver::getAudioHandler()->playAudio("data/audio/game_sfx/bush.mp3", 0.2F);
  }
}

void Bush::onCollisionExit(bcw::Collider* collision)
{
  if (collision->getAttachedScene()->getTag() == "Player")
  {
    std::dynamic_pointer_cast<CharacterBase>(collision->getAttachedScene())->setIsVisible(true);
  }
}
