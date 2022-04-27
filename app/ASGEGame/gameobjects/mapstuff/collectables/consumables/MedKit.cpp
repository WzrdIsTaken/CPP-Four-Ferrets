// Created by Ben on 24/03/2022

#include "gameobjects/characters/CharacterBase.hpp"

#include "MedKit.hpp"
#include <iostream>

void MedKit::start()
{
  CollectableBase::start();

  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/images/collectables/medpack.png", { 0, 0 }, 0.75F);
}

void MedKit::onCollisionEnter(bcw::Collider* collision)
{
  if (collision->getAttachedScene()->getTag() == "Player")
  {
    std::dynamic_pointer_cast<CharacterBase>(collision->getAttachedScene())->heal(HEAL_AMOUNT);

    bcw::Driver::getAudioHandler()->playAudio("data/audio/game_sfx/medkit.mp3", 0.2F);

    bcw::Driver::getSceneHandler()->removeGameObject(this->shared_from_this());
    std::cout << "MEDKIIIIIT \n";
    colliderInCollidables->setMarkedForRemoval(true);
    colliderInCollidables = nullptr;
  }
}

void MedKit::onCollisionExit(bcw::Collider* collision) {}
