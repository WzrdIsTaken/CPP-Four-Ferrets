// Created by Ben on 16/03/2022

#pragma once

#include "../IInteractable.hpp"
#include "../IPickable.hpp"

#include "DoorBase.hpp"

class ManualDoor
  : public DoorBase
  , public IInteractable
  , public IPickable
{
 public:
  ManualDoor()           = default;
  ~ManualDoor() override = default;

  void onInteractionBegin() override;
  void onInteractionEnd() override;

  void onPickBegin(float pick_speed) override;
  void onPickEnd() override;
  bool getPickedState() override;
};
