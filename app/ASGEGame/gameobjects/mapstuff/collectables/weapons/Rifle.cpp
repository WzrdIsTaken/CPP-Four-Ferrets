// Created by Ben on 24/03/2022

#include <ASGEGameLib/HelperMethods.hpp>

#include "Rifle.hpp"

void Rifle::start()
{
  // Attributes
  ammo                = 7;
  damage              = 30;
  delay_between_shots = 0.1;

  // Init base class
  PickupableCollectable::start();

  // Sprites
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/images/collectables/rifle.png", { 0, 0 }, 0.1F);
  sprite->rotationInRadians(bcw::MoreMath::degrees2Rad(-90.0F));

  bcw::Driver::getRenderHandler()->setupSprite(
    icon_sprite.get(), "/data/images/collectables/rifle.png", { 0, 0 }, 0.1F);
}
