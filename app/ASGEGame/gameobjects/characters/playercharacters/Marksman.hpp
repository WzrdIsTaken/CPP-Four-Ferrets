// Created by Ben on 31/03/2022

#pragma once

#include "../PlayerCharacter.hpp"

class Marksman : public PlayerCharacter
{
 public:
  Marksman()           = default;
  ~Marksman() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;

 protected:
  void ability(const bcw::Vector2& use_direction) override;
  void interact() override;
};
