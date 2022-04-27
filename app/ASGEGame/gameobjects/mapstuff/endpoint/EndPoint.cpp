// Created by Ben on 07/04/2022

#include "../../../scenes/GameScene.hpp"

#include "EndPoint.hpp"

void EndPoint::render(const ASGE::GameTime& /*game_time*/)
{
  if (show_debug_sprite)
  {
    bcw::Driver::getRenderer()->render(*sprite);
  }
}

void EndPoint::init(
  const bcw::Vector2& position, const bcw::Vector2& size, const bool _show_debug_sprite,
  const float opacity) // NOLINT
{
  sprite = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/debug/test_image.png", position);
  sprite->width(size.x);
  sprite->height(size.y);
  sprite->opacity(opacity);

  collider = std::make_unique<bcw::Collider>(
    [this](bcw::Collider* collision) { onCollisionEnter(collision); },
    [this](bcw::Collider* collision) { onCollisionExit(collision); },
    sprite.get(),
    this->shared_from_this());

  show_debug_sprite = _show_debug_sprite;
}

void EndPoint::onCollisionEnter(bcw::Collider* collision)
{
  if (collision->getAttachedScene()->getTag() == "Player")
  {
    players_at_end_point++;

    /*assert(
      players_at_end_point > 4 && "players_at_end_point is greater than 4, something has gone very "
                                  "wrong");*/

    auto level = bcw::Driver::getSceneHandler()->findScene<GameScene>();
    if (players_at_end_point == 4 && level->getObjectiveCollected())
    {
      level->levelComplete();
    }
  }
}

void EndPoint::onCollisionExit(bcw::Collider* collision)
{
  if (collision->getAttachedScene()->getTag() == "Player")
  {
    players_at_end_point--;

    /* assert(
       players_at_end_point < 0 && "players_at_end_point is less than 0, something has gone very "
                                   "wrong");*/
  }
}
