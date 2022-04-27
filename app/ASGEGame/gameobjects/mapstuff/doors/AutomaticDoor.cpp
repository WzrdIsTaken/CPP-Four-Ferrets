// Created by Ben on 16/03/2022

#include "AutomaticDoor.hpp"

void AutomaticDoor::onHackBegin(const float hack_speed)
{
  if (hack_speed > 0)
  {
    return;
  }

  locked = false;
  toggleDoor();

  // Possible issue: The doors pace it away once unlocked?
  // Logging::INFO(bcw::Vector2{sprite->xPos(), sprite->yPos()}.toString());
  // toggleUnlock(hack_speed);
}

void AutomaticDoor::onHackEnd() {}

bool AutomaticDoor::getHackedState()
{
  return locked;
}
