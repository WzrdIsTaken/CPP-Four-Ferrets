// Created by Ben on 24/03/2022

#include "../../../../scenes/GameScene.hpp"

#include "MissionObjectiveObject.hpp"

void MissionObjectiveObject::start()
{
  CollectableBase::start();

  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/images/collectables/final_item_highlight.png", { 0, 0 }, 1.0F);
  sprite->colour(ASGE::COLOURS::YELLOW);

  // I would grab the reference to levelmanager or whatever we call it here
  // This can then we used to chose the sprite that will be used, and later on
  // player collision so that we can tell the levelmanager that the players
  // have the key and are ready to escape!

  // ^ DONE!
}

void MissionObjectiveObject::onCollisionEnter(bcw::Collider* collision)
{
  if (collision->getAttachedScene()->getTag() == "Player")
  {
    auto level = bcw::Driver::getSceneHandler()->findScene<GameScene>();
    level->setObjectiveCollected(true);

    bcw::Driver::getSceneHandler()->removeGameObject(this->shared_from_this());
    colliderInCollidables->setMarkedForRemoval(true);
    colliderInCollidables = nullptr;
    bcw::Driver::getAudioHandler()->playAudio("data/audio/game_sfx/collect_end_item.mp3", 0.4F);
  }
}

void MissionObjectiveObject::onCollisionExit(bcw::Collider* /*collision*/) {}
