// Created by Ben on 17/03/2022

#pragma once

#include "../NonPlayerCharacter.hpp"

class Civilian : public NonPlayerCharacter
{
 public:
  Civilian()           = default;
  ~Civilian() override = default;

  void start() override;
};
