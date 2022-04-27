// Created by Ben on 18/03/2022

#include <ASGEGameLib/HelperMethods.hpp>

#include "../../helper/ZOrderData.hpp"

#include "ProjectileBase.hpp"

void ProjectileBase::start()
{
  sprite = bcw::Driver::getRenderer()->createUniqueSprite();
  sprite->setGlobalZOrder(ZOrderData::PROJECTILES);

  collider = std::make_unique<bcw::Collider>(
    [this](bcw::Collider* collision) { onCollisionEnter(collision); },
    [this](bcw::Collider* collision) { onCollisionExit(collision); },
    sprite.get(),
    this->shared_from_this());

  colliderInCollidables = bcw::Driver::getCollisionHandler()->getLastColliderAdded();
}

void ProjectileBase::update(const ASGE::GameTime& game_time)
{
  move(game_time.deltaInSecs());
  handleLifeTime(game_time.deltaInSecs());
}

/*
void ProjectileBase::render(const ASGE::GameTime& game_time)
{
  bcw::Driver::getRenderer()->render(*sprite);
}
*/

void ProjectileBase::init(
  ProjectileBase::Target _target, const bcw::Vector2& start_position,
  const bcw::Vector2& target_position, const int _damage) // NOLINT
{
  target = _target;
  damage = _damage;

  sprite->xPos(start_position.x);
  sprite->yPos(start_position.y);

  velocity.x = target_position.x;
  velocity.y = target_position.y;
  velocity.normalise();

  sprite->rotationInRadians(atan2(velocity.x, -velocity.y));
  // sprite->rotationInRadians(bcw::MoreMath::lookAt(start_position, target_position));
}

void ProjectileBase::init(ProjectileBase::Target _target, float rotation, int _damage)
{
  target = _target;
  damage = _damage;

  sprite->rotationInRadians(rotation);
}

bool ProjectileBase::checkIfValidTarget(bcw::Collider* collision)
{
  return ((target == ProjectileBase::Target::BOTH || target == ProjectileBase::Target::PLAYER) &&
          collision->getAttachedScene()->getTag() == "Player") ||
         ((target == ProjectileBase::Target::BOTH || target == ProjectileBase::Target::NPC) &&
          collision->getAttachedScene()->getTag() == "NPC");
}

void ProjectileBase::move(const double delta_time)
{
  sprite->xPos(static_cast<float>(sprite->xPos() + velocity.x * speed * delta_time));
  sprite->yPos(static_cast<float>(sprite->yPos() + velocity.y * speed * delta_time));
}

void ProjectileBase::handleLifeTime(const double delta_time)
{
  life_timer += delta_time;
  if (life_timer > life_time)
  {
    bcw::Driver::getSceneHandler()->removeGameObject(this->shared_from_this());
    colliderInCollidables->setMarkedForRemoval(true);
    colliderInCollidables = nullptr;
  }
}

// On collision enter
// Check tag, check target (need to assign tags to the players/npcs)
// Deal damage
// Destroy sprite