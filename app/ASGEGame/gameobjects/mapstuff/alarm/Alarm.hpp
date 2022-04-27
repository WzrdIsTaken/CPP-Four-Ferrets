// Created by Ben on 31/03/2022

#pragma once

#include <ASGEGameLib/components/animator/Animator.hpp>

#include "../MapObject.hpp"

class Alarm : public MapObject
{
 public:
  Alarm()           = default;
  ~Alarm() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;

  void init(const bcw::Vector2& position);

  void activate(bool _active);
  void disable(bool _disable);

 private:
  std::unique_ptr<bcw::Animator> animator    = nullptr;
  std::unique_ptr<ASGE::Sprite> light_sprite = nullptr;

  bool active   = false;
  bool disabled = false;

  const float SPIN_SPEED      = 4.0F;
  float light_sprite_rotation = 0.0F;
};
