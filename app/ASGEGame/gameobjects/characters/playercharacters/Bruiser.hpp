// Created by Ben on 31/03/2022

#pragma once

#include "../PlayerCharacter.hpp"

class Bruiser : public PlayerCharacter
{
 public:
  Bruiser()           = default;
  ~Bruiser() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;

 protected:
  void ability(const bcw::Vector2& use_direction) override;
  void interact() override;

 private:
  const bcw::Vector2 SLAM_AREA = { 500, 500 };
  const double STUN_TIME       = 3.0;
};
