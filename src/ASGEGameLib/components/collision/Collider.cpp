// Created by Ben on 18/03/2022

#include "../../../../include/ASGEGameLib/Driver.hpp"

#include "../../../../include/ASGEGameLib/components/collision/Collider.hpp"

bcw::Collider::Collider(
  const std::function<void(Collider*)>& _on_collision_enter, // NOLINT
  const std::function<void(Collider*)>& _on_collision_exit,  // NOLINT
  ASGE::Sprite* sprite, const std::shared_ptr<Scene>& scene) :
  on_collision_enter(_on_collision_enter),
  on_collision_exit(_on_collision_exit), check_sprite(sprite), attached_scene(scene)
{
  bcw::Driver::getCollisionHandler()->addCollider(this);
}

bcw::Collider::~Collider()
{
  bcw::Driver::getCollisionHandler()->removeCollider(this);
}

ASGE::Sprite* bcw::Collider::getAABBCheckSprite()
{
  return check_sprite;
}

std::shared_ptr<bcw::Scene> bcw::Collider::getAttachedScene()
{
  return attached_scene;
}

void bcw::Collider::setRaycastTarget(const bool _raycast_target) // NOLINT
{
  raycast_target = _raycast_target;
}

bool bcw::Collider::getRaycastTarget() const
{
  return raycast_target;
}

void bcw::Collider::setMarkedForRemoval(const bool _marked_for_removal) // NOLINT
{
  marked_for_removal = _marked_for_removal;
}

bool bcw::Collider::getMarkedForRemoval() const
{
  return marked_for_removal;
}

void bcw::Collider::setBoolWall(const bool _bool_wall) // NOLINT
{
  bool_wall = _bool_wall;
}

bool bcw::Collider::getBoolWall() const
{
  return bool_wall;
}
