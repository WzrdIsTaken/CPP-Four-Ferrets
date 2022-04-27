// Raycast base logic by James on 07/04/2022
// Made into a component by Ben on 07/04/2022

// Why is this not in the CollisionHandler? See the comment at the top of CollisionHandler.hpp

#pragma once

#include "HitResult.hpp"

namespace bcw
{
  class [[maybe_unused]] Raycast
  {
   public:
    Raycast() = default;
    explicit Raycast(bcw::Collider* _my_collider);
    ~Raycast() = default;

    HitResult
    castRay(const bcw::Vector2& origin, const bcw::Vector2& direction, float range = 9999);

   private:
    bool rayVsRect(
      const bcw::Vector2& origin, const bcw::Vector2& dir, bcw::Collider* target,
      bcw::Vector2& contact_point, float& t);

    Collider* my_collider = nullptr;

    // void castRay(const bcw::Vector2& origin, bcw::Vector2& dir);
    // std::vector<int> vec_map;
    // bcw::Vector2Int map_size;
  };
}
