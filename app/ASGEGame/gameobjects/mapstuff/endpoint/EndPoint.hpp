// Created by Ben on 07/04/2022

#pragma once

#include "../MapObject.hpp"

class EndPoint : public MapObject
{
 public:
  EndPoint()           = default;
  ~EndPoint() override = default;

  void render(const ASGE::GameTime& game_time) override;

  void init(
    const bcw::Vector2& position, const bcw::Vector2& size, bool _show_debug_sprite = false,
    float opacity = 1.F);

 private:
  void onCollisionEnter(bcw::Collider* collision);
  void onCollisionExit(bcw::Collider* collision);

  std::unique_ptr<bcw::Collider> collider;

  bool show_debug_sprite = false;

  int players_at_end_point = 0;
};
