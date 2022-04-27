// Created by Ben on 01/04/2022

#include "../../doors/LockedDoor.hpp"

#include "Key.hpp"

void Key::start()
{
  // Attributes
  picked_up_offset.x = -7.5F;
  picked_up_offset.y = 7.5F;

  // Init base class
  PickupableCollectable::start();

  // Sprite
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "data/images/key/key.png", { 0, 0 }, 0.5F);

  // Tag
  setTag("Key");
}

void Key::init(const bcw::Vector2& position, const IKeyUnlockable::KeyColour _key_colour) // NOLINT
{
  CollectableBase::init(position);

  switch (_key_colour)
  {
    case IKeyUnlockable::KeyColour::RED:
      sprite->colour(ASGE::COLOURS::RED);
      break;
    case IKeyUnlockable::KeyColour::BLUE:
      sprite->colour(ASGE::COLOURS::BLUE);
      break;
    case IKeyUnlockable::KeyColour::GREEN:
      sprite->colour(ASGE::COLOURS::GREEN);
      break;
    case IKeyUnlockable::KeyColour::YELLOW:
      sprite->colour(ASGE::COLOURS::YELLOW);
      break;
  }
  key_colour = _key_colour;
}

void Key::onUseBegin(const bcw::Vector2& /*use_direction*/) // use_direction could be used for dot
                                                            // product
{
  // because no raycasting ;;;;_;

  std::vector<bcw::Collider*> collisions =
    bcw::Driver::getCollisionHandler()->checkCollisionsInArea(
      sprite->xPos(), sprite->yPos(), USE_AREA.x, USE_AREA.y);

  for (const auto& collider : collisions)
  {
    if (
      collider->getAttachedScene() != nullptr && collider->getAttachedScene()->getTag() == "KeyUnlo"
                                                                                           "ckedDoo"
                                                                                           "r")
    {
      std::shared_ptr<LockedDoor> door =
        std::dynamic_pointer_cast<LockedDoor>(collider->getAttachedScene());
      if (door->key_colour == this->key_colour) // this-> for clarity
      {
        door->unlock();
        removeCollectableFromPlayersInventory();

        return;
      }
    }
  }
}

void Key::onUseEnd(const bcw::Vector2& use_direction) {}

void Key::onCollisionEnter(bcw::Collider* collision)
{
  PickupableCollectable::onCollisionEnter(collision);
  bcw::Driver::getAudioHandler()->playAudio("data/audio/game_sfx/key_pickup.mp3", 0.8F);
}