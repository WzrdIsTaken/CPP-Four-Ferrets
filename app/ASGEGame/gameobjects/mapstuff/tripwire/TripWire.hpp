// Created by Ben on 31/03/2022.

#pragma once

#include <ASGEGameLib/components/animator/Animator.hpp>
#include <ASGEGameLib/components/collision/Collider.hpp>

#include "../MapObject.hpp"

class TripWire : public MapObject
{
 public:
  TripWire()           = default;
  ~TripWire() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;

  void init(const bcw::Vector2& position);

 private:
  void onCollisionEnter(bcw::Collider* collision);
  void onCollisionExit(bcw::Collider* collision);

  std::unique_ptr<bcw::Animator> animator;
  std::unique_ptr<bcw::Collider> collider;

  bool tripped = false;
};
