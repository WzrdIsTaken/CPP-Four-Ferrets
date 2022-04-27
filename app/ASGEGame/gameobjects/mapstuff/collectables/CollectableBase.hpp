// Created by Ben on 24/03/2022

#pragma once

#include <ASGEGameLib/components/collision/Collider.hpp>

#include "../MapObject.hpp"

class CollectableBase : public MapObject
{
 public:
  CollectableBase()           = default;
  ~CollectableBase() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;

  void init(const bcw::Vector2& spawn_position);

 protected:
  virtual void onCollisionEnter(bcw::Collider* collision) = 0;
  virtual void onCollisionExit(bcw::Collider* collision)  = 0;

  std::unique_ptr<bcw::Collider> collider = nullptr;
  bcw::Collider* colliderInCollidables    = nullptr;

  float top_y = 0.0F;
  float bot_y = 0.0F;

  float move_dir             = 1.0F;
  const float UP_DOWN_AMOUNT = 10.0F;
  const float UP_DOWN_SPEED  = 10.0F;
};
