// Created by Ben on 18/03/2022

#pragma once

#include <ASGEGameLib/Driver.hpp>
#include <ASGEGameLib/components/collision/Collider.hpp>

#include "../mapstuff/MapObject.hpp"

class ProjectileBase : public MapObject // bot fix...
{
 public:
  ProjectileBase()           = default;
  ~ProjectileBase() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  // void render(const ASGE::GameTime& game_time) override;

  enum class Target
  {
    PLAYER,
    NPC,
    BOTH
  }; // Changes what the projectile will collide with
  void init(
    Target _target, const bcw::Vector2& start_position, const bcw::Vector2& target_direction,
    int _damage);
  void init(Target _target, float rotation, int _damage);

 protected:
  virtual void onCollisionEnter(bcw::Collider* collision) = 0;
  virtual void onCollisionExit(bcw::Collider* collision)  = 0;

  bool checkIfValidTarget(bcw::Collider* collision);

  // std::unique_ptr<ASGE::Sprite> sprite;
  std::unique_ptr<bcw::Collider> collider;

  // Attributes
  float speed = 0;
  int damage  = 0;

  bcw::Collider* colliderInCollidables = nullptr;

 private:
  void move(double delta_time);
  void handleLifeTime(double delta_time);

  bcw::Vector2 velocity;
  Target target = Target::BOTH;

  // Just to make sure if the projectile doesn't collide with anything, it won't exist forever
  double life_time  = 10.0;
  double life_timer = 0.0;
};
