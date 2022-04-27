// Created by Ben on 16/03/2022

#include <cmath>

#include "../../include/ASGEGameLib/HelperMethods.hpp"

// _USE_MATH_DEFINES causes werror, and I can't seem to get pi anywhere else.
// Though I probably just missed it...
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062F

float bcw::MoreMath::lookAt(const bcw::Vector2& current, const bcw::Vector2& target)
{
  float dx = target.x - current.x;
  float dy = target.y - current.y;
  return atan2(dy, dx);
}

float bcw::MoreMath::rad2Degrees(const float rad)
{
  return rad * (180.0F / PI);
}

float bcw::MoreMath::degrees2Rad(const float degrees)
{
  return degrees * (PI / 180.0F);
}
