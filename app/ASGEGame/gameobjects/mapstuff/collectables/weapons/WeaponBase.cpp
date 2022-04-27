// Created by Ben on 24/03/2022

#include "../../../projectiles/Bullet.hpp"

#include "WeaponBase.hpp"

void WeaponBase::start()
{
  PickupableCollectable::start();

  setTag("Weapon");
}

void WeaponBase::update(const ASGE::GameTime& game_time)
{
  CollectableBase::update(game_time);

  shoot_cool_down += game_time.deltaInSecs();
}

void WeaponBase::shoot(const bcw::Vector2& direction)
{
  if (shoot_cool_down > delay_between_shots)
  {
    std::shared_ptr<Bullet> bullet = bcw::Driver::getSceneHandler()->addGameObject<Bullet>();
    bullet->init(
      ProjectileBase::Target::NPC, { sprite->xPos(), sprite->yPos() }, direction, damage);

    shoot_cool_down = 0.0;
    ammo--;

    if (ammo <= 0)
    {
      removeCollectableFromPlayersInventory();
    }
  }
}

void WeaponBase::onUseBegin(const bcw::Vector2& use_direction)
{
  shoot(use_direction);
}

void WeaponBase::onUseEnd(const bcw::Vector2& use_direction) {}

void WeaponBase::onCollisionEnter(bcw::Collider* collision)
{
  PickupableCollectable::onCollisionEnter(collision);
  if (is_picked_up)
  {
    bcw::Driver::getAudioHandler()->playAudio("data/audio/game_sfx/weapon_pickup.mp3", 0.8F);
  }
}
