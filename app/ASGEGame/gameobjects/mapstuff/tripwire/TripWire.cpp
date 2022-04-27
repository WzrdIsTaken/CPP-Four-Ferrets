// Created by Ben on 31/03/2022

#include "TripWire.hpp"

void TripWire::start()
{
  // Init base class
  MapObject::start();

  // Sprite
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "data/images/tripwire/tripwire_1.png", { 0, 0 }, 1.5F);

  // Animator
  std::map<std::string, std::vector<std::string>> animations = {
    { "normal", { "data/images/tripwire/tripwire_1.png", "data/images/tripwire/tripwire_2.png" } },
    { "broken", { "data/images/tripwire/tripwire_broken.png" } }
  };
  animator = std::make_unique<bcw::Animator>(sprite.get(), animations, 0.12F);
  animator->setAnimation("idle");

  // Collider
  collider = std::make_unique<bcw::Collider>(
    [this](bcw::Collider* collision) { onCollisionEnter(collision); },
    [this](bcw::Collider* collision) { onCollisionExit(collision); },
    sprite.get(),
    this->shared_from_this());
}

void TripWire::update(const ASGE::GameTime& game_time)
{
  animator->update(game_time);
}

void TripWire::init(const bcw::Vector2& position)
{
  sprite->xPos(position.x);
  sprite->yPos(position.y);
}

void TripWire::onCollisionEnter(bcw::Collider* collision)
{
  if (collision->getAttachedScene()->getTag() == "Player" && !tripped)
  {
    tripped = true;
    animator->setAnimation("broken");
  }
}

void TripWire::onCollisionExit(bcw::Collider* collision) {}
