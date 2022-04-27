// Created by Ben on 16/03/2022

#pragma once

#include <ASGEGameLib/Driver.hpp>

class MapObject : public bcw::Scene
{
 public:
  MapObject()           = default;
  ~MapObject() override = default;

  void start() override;
  void render(const ASGE::GameTime& game_time) override;

  // i don't know how james lighting stuff might work, but he may well need the MapObject' sprites
  // and anyway I need PickupableCollectable's sprites right now for the hacker so might as well
  // future proof...
  ASGE::Sprite* getSprite();

 protected:
  std::unique_ptr<ASGE::Sprite> sprite;
};