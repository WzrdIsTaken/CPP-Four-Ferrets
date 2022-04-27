// Created by Ben on 24/03/2022

#pragma once

#include "../PickupableCollectable.hpp"

class WeaponBase : public PickupableCollectable
{
 public:
  WeaponBase()           = default;
  ~WeaponBase() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;

  void onUseBegin(const bcw::Vector2& use_direction) override;
  void onUseEnd(const bcw::Vector2& use_direction) override;

  void shoot(const bcw::Vector2& direction);

 protected:
  int ammo                   = 0;
  int damage                 = 0;
  double delay_between_shots = 0;

 private:
  void onCollisionEnter(bcw::Collider* collision) override;
  double shoot_cool_down = 0;
};

// If you have a weapon, then pick up another weapon, the second weapon will have infinite ammo???
// ^ sometimes kek. or maybe I fixed it now ;p
