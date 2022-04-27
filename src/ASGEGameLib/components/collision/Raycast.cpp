// Raycast base logic by James on 07/04/2022
// Made into a component by Ben on 07/04/2022

#include "Driver.hpp"

#include "../../../../include/ASGEGameLib/components/collision/Raycast.hpp"

bcw::Raycast::Raycast(bcw::Collider* _my_collider) : my_collider(_my_collider) {}

bcw::HitResult
bcw::Raycast::castRay(const bcw::Vector2& origin, const bcw::Vector2& direction, const float range)
{
  bcw::Vector2 contact_point{ 0, 0 };
  float t      = 0;                            // ?
  auto sort_hr = [&](HitResult a, HitResult b) // ben seal of approval
  { return a.time < b.time; };

  std::vector<HitResult> results;
  for (const auto& collider : bcw::Driver::getCollisionHandler()->getAllCollidables())
  {
    // Bot fix to make up for idk dude just unlucky

    if (collider != my_collider && collider->getRaycastTarget())
    {
      ASGE::Sprite* collider_sprite = collider->getAABBCheckSprite();
      if (origin.distance({ collider_sprite->xPos(), collider_sprite->yPos() }) > range)
      {
        continue;
      }

      if (rayVsRect(origin, direction, collider, contact_point, t))
      {
        HitResult& result = results.emplace_back();
        result.hit        = collider->getBoolWall() ? nullptr : collider;
        // result.hit            = collider; // This line was in Ben's code before the raycast has
        // been fixed
        result.contact_point  = contact_point;
        result.contact_normal = { 1, 1 }; // meaningless atm (sorry contact_normal)
        result.time           = t;
      }
    }
  }

  if (!results.empty())
  {
    std::sort(results.begin(), results.end(), sort_hr);
    return results.front();
  }

  return {};
}

bool bcw::Raycast::rayVsRect(
  const bcw::Vector2& origin, const bcw::Vector2& dir, bcw::Collider* target,
  bcw::Vector2& contact_point, float& t) // why is this a reference ??
{
  bcw::Vector2 inverse_dir = { 1.0F / dir.x, 1.0F / dir.y };
  const auto& sprite       = target->getAABBCheckSprite();
  bcw::Vector2 near_t = (bcw::Vector2{ sprite->xPos(), sprite->yPos() } - origin) * inverse_dir;
  bcw::Vector2 far_t  = (bcw::Vector2{ sprite->xPos(), sprite->yPos() } +
                        bcw::Vector2{ sprite->width(), sprite->height() } - origin) *
                       inverse_dir;

  bcw::Vector2 contact_normal;

  // if a divide by 0 has occurred and numbers are irrational, return false
  if (std::isnan(far_t.x) || std::isnan(far_t.y))
  {
    return false;
  }
  if (std::isnan(near_t.x) || std::isnan(near_t.y))
  {
    return false;
  }

  // sort near and far distances
  if (near_t.x > far_t.x)
  {
    std::swap(near_t, far_t);
  }
  if (near_t.y > far_t.y)
  {
    std::swap(near_t.y, far_t.y);
  }

  if (near_t.x > far_t.y || near_t.y > far_t.x)
  {
    return false;
  }

  t               = near_t.maximum();
  float t_far_hit = far_t.minimum();
  if (t_far_hit < 0.0F)
  {
    return false;
  }
  contact_point = origin + t * dir;
 //contact_point.x = origin.x + (t * dir.x);
  //contact_point.y = origin.y + (t * dir.y);
  if (near_t.x > near_t.y)
  {
    if (inverse_dir.x < 0)
    {
      contact_normal = { 1, 0 };
    }
    else
    {
      contact_normal = { -1, 0 };
    }
  }
  else if (near_t.x < near_t.y)
  {
    if (inverse_dir.y < 0)
    {
      contact_normal = { 0, 1 };
    }
    else
    {
      contact_normal = { 0, -1 };
    }
  }

  return true;
}

/*
bcw::HitResult bcw::Raycast::castRay(const bcw::Vector2& origin, bcw::Vector2& dir)
{
  bcw::Vector2Int map_check {static_cast<int>(origin.x), static_cast<int>(origin.y)};
  bcw::Vector2 norm_dir = dir.normalise();
  bcw::Vector2 inv_dir = {1/dir.x, 1/dir.y};
  bcw::Vector2 step_size = {sqrt(1+(norm_dir.y/norm_dir.x) * (norm_dir.y/norm_dir.x)),
sqrt(1+(norm_dir.x/norm_dir.y) * (norm_dir.x/norm_dir.y) )}; bcw::Vector2 length_1d;

bcw::Vector2 contact_normal;
bcw::Vector2Int step = {0,0};

if(norm_dir.x < 0)
{
  step.x = -1;
  length_1d.x = (origin.x -(static_cast<float>(map_check.x))) * step_size.x;
}
else
{
  step.x = 1;
  length_1d.x = (static_cast<float>(map_check.x+1)-origin.x) * step_size.x;
}
if(norm_dir.y < 0)
{
  step.y = -1;
  length_1d.y = (origin.y-(static_cast<float>(map_check.y))) * step_size.y;
}
else
{
  step.y = 1;
  length_1d.y = (static_cast<float>(map_check.y)-origin.y) * step_size.y;
}

bool found = false;
float max_dist = 1000;
float distance = 0;
while(!found && distance < max_dist)
{
  if (length_1d.x < length_1d.y)
  {
    //step along in X
    map_check.x += step.x;
    distance = length_1d.x;
    length_1d.x += step_size.x;
  }
  else
  {
    //step along in y
    map_check.y += step.y;
    distance = length_1d.y;
    length_1d.y += step_size.y;
  }

  if(map_check.x >= 0 && map_check.x < map_size.x && map_check.y >= 0 && map_check.y < map_size.y )
  {
    if(vec_map[static_cast<std::size_t>(map_check.y * map_size.y + map_check.x)] == 1)
    {
      found = true;

    }
  }
}

bcw::Vector2 intersection;
if(found)
{
  intersection = {origin.x + norm_dir.x * distance, origin.y + norm_dir.y * distance};
  if(length_1d.x > length_1d.y)
  {
    if(inv_dir.x < 0)
    {
      contact_normal = {1,0};
    }
    else
    {
      contact_normal = {-1,0};
    }
  }
  else if(length_1d.x < length_1d.y)
  {
    if(inv_dir.y < 0)
    {
      contact_normal = {0,1};

    }
    else if(inv_dir.y > 0)
    {
      contact_normal = {0,-1};
    }
  }
}

return {};
}
*/
