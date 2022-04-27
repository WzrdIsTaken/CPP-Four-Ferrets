// Created by Ben on 01/04/2022

#pragma once

#include <ASGEGameLib/components/collision/Collider.hpp>

#include "../../IKeyUnlockable.hpp"
#include "../PickupableCollectable.hpp"

class Key
  : public PickupableCollectable
  , public IKeyUnlockable
{
 public:
  Key()           = default;
  ~Key() override = default;

  void start() override;
  void init(const bcw::Vector2& position, IKeyUnlockable::KeyColour _key_colour);

  void onUseBegin(const bcw::Vector2& use_direction) override;
  void onUseEnd(const bcw::Vector2& use_direction) override;

 private:
  const bcw::Vector2 USE_AREA = { 100, 100 };
  void onCollisionEnter(bcw::Collider* collision) override;
};
