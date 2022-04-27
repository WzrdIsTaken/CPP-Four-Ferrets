// Created by Ben on 16/03/2022

#include "../../helper/ZOrderData.hpp"

#include "MapObject.hpp"

void MapObject::start()
{
  sprite = bcw::Driver::getRenderer()->createUniqueSprite();
  sprite->setGlobalZOrder(ZOrderData::MAP_OBJECT);
}

void MapObject::render(const ASGE::GameTime& /*game_time*/)
{
  bcw::Driver::getRenderer()->render(*sprite);
}

ASGE::Sprite* MapObject::getSprite()
{
  return sprite.get();
}
