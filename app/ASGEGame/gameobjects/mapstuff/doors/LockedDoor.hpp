// Created by Ben on 01/04/2022

#pragma once

#include "../IInteractable.hpp"
#include "../IKeyUnlockable.hpp"

#include "DoorBase.hpp"

class LockedDoor
  : public DoorBase
  , public IInteractable
  , public IKeyUnlockable
{
 public:
  LockedDoor()           = default;
  ~LockedDoor() override = default;

  void start() override;
  void setUnlockColour(IKeyUnlockable::KeyColour _key_colour);

  void unlock();

  void onInteractionBegin() override;
  void onInteractionEnd() override;
};
