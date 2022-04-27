// Created by Ben on 16/03/2022

#include "ManualDoor.hpp"

void ManualDoor::onInteractionBegin()
{
  toggleDoor();
}

void ManualDoor::onInteractionEnd() {}

void ManualDoor::onPickBegin(const float pick_speed)
{
  toggleUnlock(pick_speed);
}

void ManualDoor::onPickEnd() {}

bool ManualDoor::getPickedState()
{
  return locked;
}
