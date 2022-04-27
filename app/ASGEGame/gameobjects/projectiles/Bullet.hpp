// Created by Ben on 18/03/2022

#pragma once

#include "ProjectileBase.hpp"

class Bullet : public ProjectileBase
{
 public:
  Bullet()           = default;
  ~Bullet() override = default;

  void start() override;

 protected:
  void onCollisionEnter(bcw::Collider* collision) override;
  void onCollisionExit(bcw::Collider* collision) override;
};