// Created by Ben on 26/04/2022

#pragma once

struct ZOrderData
{
  [[maybe_unused]] static const short ZERO         = 0;
  [[maybe_unused]] static const short MAP          = 1;
  [[maybe_unused]] static const short MAP_OBJECT   = 10;
  [[maybe_unused]] static const short COLLECTABLES = 15;
  [[maybe_unused]] static const short CHARACTERS   = 16;
  [[maybe_unused]] static const short PROJECTILES  = 17;
  [[maybe_unused]] static const short POP_UPS      = 18;
  [[maybe_unused]] static const short LIGHTS       = 100;
  [[maybe_unused]] static const short UI           = 101;
};
