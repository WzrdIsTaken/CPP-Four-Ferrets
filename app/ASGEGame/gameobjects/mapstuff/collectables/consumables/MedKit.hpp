// Created by Ben on 24/03/2022

#pragma once

#include "gameobjects/mapstuff/collectables/CollectableBase.hpp"

class MedKit : public CollectableBase
{
 public:
  MedKit()           = default;
  ~MedKit() override = default;

  void start() override;

 protected:
  void onCollisionEnter(bcw::Collider* collision) override;
  void onCollisionExit(bcw::Collider* collision) override;

 private:
  const int HEAL_AMOUNT = 50;
};
