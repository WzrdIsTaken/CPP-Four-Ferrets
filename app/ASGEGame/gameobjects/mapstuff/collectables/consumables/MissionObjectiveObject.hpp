// Created by Ben on 24/03/2022

#pragma once

#include "gameobjects/mapstuff/collectables/CollectableBase.hpp"

class MissionObjectiveObject : public CollectableBase
{
 public:
  MissionObjectiveObject()           = default;
  ~MissionObjectiveObject() override = default;

  void start() override;

 protected:
  void onCollisionEnter(bcw::Collider* collision) override;
  void onCollisionExit(bcw::Collider* collision) override;
};
