// Created by Ben on 24/03/2022

#pragma once

#include "WeaponBase.hpp"

class Rifle : public WeaponBase
{
 public:
  Rifle()           = default;
  ~Rifle() override = default;

  void start() override;
};
