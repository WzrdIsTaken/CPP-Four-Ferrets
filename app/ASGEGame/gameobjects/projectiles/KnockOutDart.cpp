// Created by Ben on 18/03/2022

#include "../characters/CharacterBase.hpp"

#include "KnockOutDart.hpp"

void KnockOutDart::start()
{
  // Attributes
  damage = 0;
  speed  = 300.0F;

  // Init base class
  ProjectileBase::start();

  // Sprite
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/images/projectiles/dart.png", { 1, 1 }, 0.5F);
  sprite->setGlobalZOrder(16);
}

void KnockOutDart::onCollisionEnter(bcw::Collider* collision)
{
  if (checkIfValidTarget(collision))
  {
    std::dynamic_pointer_cast<CharacterBase>(collision->getAttachedScene())
      ->knockOut(KNOCK_OUT_TIME);

    bcw::Driver::getSceneHandler()->removeGameObject(this->shared_from_this());
    colliderInCollidables->setMarkedForRemoval(true);
    colliderInCollidables = nullptr;
  }
}

void KnockOutDart::onCollisionExit(bcw::Collider* collision) {}
