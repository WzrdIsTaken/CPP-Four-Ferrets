// Created by Ben on 17/03/2022

#include "Civilian.hpp"

void Civilian::start()
{
  // Attributes
  speed = 50.0F;

  // Init base class
  NonPlayerCharacter::start();

  // Sprite / Animator
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/debug/test_image.png", { 100, 100 }, 0.75F);

  std::map<std::string, std::vector<std::string>> animations = { { "idle",
                                                                   { "/data/debug/"
                                                                     "test_image.png" } } };

  animator = std::make_unique<bcw::Animator>(sprite.get(), animations, 0.12F);
}
