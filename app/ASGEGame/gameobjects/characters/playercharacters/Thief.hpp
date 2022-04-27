// Created by Ben on 10/03/2022

#pragma once

#include "../PlayerCharacter.hpp"

class Thief : public PlayerCharacter
{
 public:
  Thief()           = default;
  ~Thief() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;

 protected:
  void ability(const bcw::Vector2& use_direction) override;
  void interact() override;

 private:
  // const float NEW_SPEED_BOOST_SPEED = 400.0F;
  // There are forces beyond our mortal understanding in this world. Creatures lurking in the
  // cracks in reality and shadows of the night. Today, at approximately 11:30am on 26/04/2022
  // we found a new one. SPEED_BOOST_SPEED. This unknown entity could not be killed and affected
  // our code in mysterious ways. Despite its removal, renaming and various watch and breakpoints
  // placed on its line, it would not give in and continued to shine its infernal presence
  // upon the project.

  // Finally we managed to subdue it with the ancient power of 'magic number', but we live in
  // fear of its awakening to this day. When will it return? Will it enact revenge for being
  // slighted on this day? The unfortunate answer is that we just don't know.

  float NORMAL_SPEED            = 0.0F; // NOLINT would be a const if it could...

  const double ABILITY_DURATION = 3.0;
  double ability_duration_clock = 0.0;

  const float PICK_DURATION = 3.0F;
};
