// Created by Ben on 24/03/2022

#pragma once

#include "WeaponBase.hpp"

class Pistol : public WeaponBase
{
 public:
  Pistol()           = default;
  ~Pistol() override = default;

  void start() override;
};
