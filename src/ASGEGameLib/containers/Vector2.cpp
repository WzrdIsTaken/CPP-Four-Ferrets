// Created by Ben on 01/02/2022

#include <cmath>

#include "../../../include/ASGEGameLib/containers/Vector2.hpp"

// --- Vector 2 ---

bcw::Vector2 bcw::Vector2::normalise()
{
  float value = 1.0F / std::sqrt((x * x) + (y * y));
  x *= value;
  y *= value;
  return { x, y };
}

bcw::Vector2 bcw::Vector2::dotProduct(Vector2& v2)
{
  return Vector2{ std::abs(x) * std::abs(v2.x), std::abs(y) * std::abs(v2.y) };
}
float bcw::Vector2::magnitude()
{
  return(sqrt((x*x) + (y*y)));
}
float bcw::Vector2::maximum()
{
  return (this->x < this->y) ? y : x;
}
float bcw::Vector2::minimum()
{
  return (this->y < this->x) ? y : x;
}

// --- Vector 2 int ---

bcw::Vector2Int::Vector2Int(int _x, int _y) : x(_x), y(_y) {}

int bcw::Vector2Int::distance(const bcw::Vector2Int& other) const
{
  int v1 = x - other.x;
  int v2 = y - other.y;
  return abs(static_cast<int>(sqrt((v1 * v1) + (v2 * v2))));
}

std::string bcw::Vector2Int::toString() const
{
  return std::string("x: " + std::to_string(x) + " y: " + std::to_string(y));
}
