// Created by Ben on 24/03/2022

#include <ASGEGameLib/HelperMethods.hpp>

#include <Engine/Logger.hpp>

#include "Pistol.hpp"

void Pistol::start()
{
  // Attributes
  ammo                = 3;
  damage              = 25;
  delay_between_shots = 0.25;

  // Init base class
  PickupableCollectable::start();

  // Sprites
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/images/collectables/pistol.png", { 0, 0 }, 0.5F);

  bcw::Driver::getRenderHandler()->setupSprite(
    icon_sprite.get(), "/data/images/collectables/pistol.png", { 0, 0 }, 0.5F);
  sprite->rotationInRadians(bcw::MoreMath::degrees2Rad(-90.0F));

  Logging::ERRORS("ngl the pistol sprite is kinda jank atm, you should probs just use the rifle!");
}
