// Created by Ben on 18/03/2022

#include "../characters/CharacterBase.hpp"

#include "Bullet.hpp"

void Bullet::start()
{
  // Attributes
  damage = 25;
  speed  = 250.0F;

  // Init base class
  ProjectileBase::start();

  // Sprite
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/images/projectiles/bullet.png", { 1, 1 }, 0.5F);
  sprite->setGlobalZOrder(16);
}

void Bullet::onCollisionEnter(bcw::Collider* collision)
{
  if (checkIfValidTarget(collision))
  {
    std::dynamic_pointer_cast<CharacterBase>(collision->getAttachedScene())->takeDamage(damage);

    bcw::Driver::getSceneHandler()->removeGameObject(this->shared_from_this());
  }
}

void Bullet::onCollisionExit(bcw::Collider* collision) {}
