// Created by Ben on 17/03/2022

#pragma once

#include <ASGEGameLib/Driver.hpp>
#include <ASGEGameLib/components/animator/Animator.hpp>

class Timer : public bcw::Scene
{
 public:
  Timer()           = default;
  ~Timer() override = default;

  void init(const bcw::Vector2& position, const ASGE::Colour& colour, double duration);
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;

 private:
  std::unique_ptr<ASGE::Sprite> sprite    = nullptr;
  std::unique_ptr<bcw::Animator> animator = nullptr;

  double duration = 0.0;
};