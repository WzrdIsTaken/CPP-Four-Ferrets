// Created by Ben on 16/03/2022

#pragma once

#include "containers/Vector2.hpp"

namespace bcw
{
  class MoreMath
  {
   public:
    static float lookAt(const bcw::Vector2& current, const bcw::Vector2& target);
    static float rad2Degrees(float rad);
    static float degrees2Rad(float degrees);
  };
} // namespace bcw
