// Created by James and Ben on [a date]

#pragma once

#include "../../containers/Vector2.hpp"
#include "Collider.hpp"

namespace bcw
{
  struct HitResult
  {
    HitResult()  = default;
    ~HitResult() = default;

    bcw::Collider* hit = nullptr;
    bcw::Vector2 contact_point;
    bcw::Vector2 contact_normal;
    float time;
  };
}
