// Created by Ben on 01/04/2022

#include "../../characters/CharacterBase.hpp"
//#include "../collectables/key/Key.hpp"

#include "LockedDoor.hpp"

void LockedDoor::start()
{
  DoorBase::start();
  setTag("KeyUnlockedDoor");
}

void LockedDoor::setUnlockColour(const IKeyUnlockable::KeyColour _key_colour) // NOLINT
{
  key_colour = _key_colour;

  switch (key_colour)
  {
    case KeyColour::RED:
      sprite->colour(ASGE::COLOURS::RED);
      break;
    case KeyColour::BLUE:
      sprite->colour(ASGE::COLOURS::BLUE);
      break;
    case KeyColour::GREEN:
      sprite->colour(ASGE::COLOURS::GREEN);
      break;
    case KeyColour::YELLOW:
      sprite->colour(ASGE::COLOURS::YELLOW);
      break;
  }
}

void LockedDoor::unlock()
{
  if (locked)
  {
    locked = false;
    toggleDoor();
    bcw::Driver::getAudioHandler()->playAudio("data/audio/game_sfx/use_key.mp3", 0.4F);
  }
}

void LockedDoor::onInteractionBegin()
{
  /*
  std::vector<bcw::Collider*> collisions =
    bcw::Driver::getCollisionHandler()->checkCollisionsInArea(
      sprite->xPos(), sprite->yPos(), 100, 100, false);

  for (const auto& col : collisions)
  {
    if (col->getAttachedScene()->getTag() == "Key")
    {
      if (std::dynamic_pointer_cast<Key>(col->getAttachedScene())->key_colour == this->key_colour)
      {
        if (locked) // should never be able to close key doors
        {
          locked = false;
          toggleDoor();
        }
      }
    }
  }
  */
}

void LockedDoor::onInteractionEnd() {}
