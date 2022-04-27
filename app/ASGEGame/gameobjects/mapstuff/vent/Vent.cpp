//
// Created by teobo on 14/04/2022.
//

#include <Engine/Logger.hpp>

#include "../../characters/CharacterBase.hpp"

#include "Vent.hpp"

void Vent::start()
{
  // Init base class - our sprite has been created
  MapObject::start();

  // Setup our unique sprite
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/debug/test_image.png", { 1, 1 }, 0.25F);

  // Collision
  collider = std::make_unique<bcw::Collider>(
    [this](bcw::Collider* collision) { onCollisionEnter(collision); },
    [this](bcw::Collider* collision) { onCollisionExit(collision); },
    sprite.get(),
    this->shared_from_this());
}

void Vent::init(const bcw::Vector2& start_position, const bcw::Vector2& _vent_position)
{
  // Setting the initial position of our sprite because we cannot have constructors ):
  sprite->xPos(start_position.x);
  sprite->yPos(start_position.y);

  // Setting the vent position
  vent_position = _vent_position;
}

void Vent::onCollisionEnter(bcw::Collider* collision)
{
  if (collision->getAttachedScene()->getTag() == "Player")
  {
    auto player = std::dynamic_pointer_cast<CharacterBase>(collision->getAttachedScene());
    player->getSprite()->xPos(vent_position.x);
    player->getSprite()->yPos(vent_position.y);
  }
}

void Vent::onCollisionExit(bcw::Collider* /*collision*/)
{
  /*
  if (collision->getAttachedScene()->getTag() == "Player")
  {
     Logging::INFO("Goodbye!");
  }
   */
}
