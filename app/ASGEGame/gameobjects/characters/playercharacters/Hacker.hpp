// Created by Ben on 10/03/2022

#pragma once

#include "../PlayerCharacter.hpp"

class Hacker : public PlayerCharacter
{
 public:
  Hacker()           = default;
  ~Hacker() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;

 protected:
  void ability(const bcw::Vector2& use_direction) override;
  void interact() override;

 private:
  void makeHackerAndHeldItemsInvisible(float invisibility_value);

  const float NORMAL_ALPHA_VALUE    = 1.0F;
  const float INVISIBLE_ALPHA_VALUE = 0.25F; // could be a cool shader soontm?

  const double ABILITY_DURATION = 3.0;
  double ability_duration_clock = 0.0;

  const float HACK_DURATION = 3.0F;
};
