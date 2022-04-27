// Created by Ben on 24/03/2022

#include "../../../helper/ZOrderData.hpp"

#include "CollectableBase.hpp"

void CollectableBase::start()
{
  MapObject::start();
  sprite->setGlobalZOrder(ZOrderData::COLLECTABLES);

  collider = std::make_unique<bcw::Collider>(
    [this](bcw::Collider* collision) { onCollisionEnter(collision); },
    [this](bcw::Collider* collision) { onCollisionExit(collision); },
    sprite.get(),
    this->shared_from_this());

  collider->setRaycastTarget(false);
  colliderInCollidables = bcw::Driver::getCollisionHandler()->getLastColliderAdded();
}

void CollectableBase::update(const ASGE::GameTime& game_time)
{
  Scene::update(game_time);

  sprite->yPos(
    static_cast<float>(sprite->yPos() + UP_DOWN_SPEED * move_dir * game_time.deltaInSecs()));
  if (sprite->yPos() > top_y)
  {
    move_dir = -1.0F;
  }
  else if (sprite->yPos() < bot_y)
  {
    move_dir = 1.0F;
  }
}

void CollectableBase::init(const bcw::Vector2& spawn_position)
{
  sprite->xPos(spawn_position.x);
  sprite->yPos(spawn_position.y);

  top_y = spawn_position.y + UP_DOWN_AMOUNT;
  bot_y = spawn_position.y - UP_DOWN_AMOUNT;
}
