// Created by Ben on 16/03/2022

#pragma once

#include "../IHackable.hpp"
#include "DoorBase.hpp"

class AutomaticDoor
  : public DoorBase
  , public IHackable
{
 public:
  AutomaticDoor()           = default;
  ~AutomaticDoor() override = default;

  // Todo: Methods to detect when a player / npc (CharacterBase) gets close and if so open
  // (and moves away -> close)

  void onHackBegin(float hack_speed) override;
  void onHackEnd() override;
  bool getHackedState() override;
};
