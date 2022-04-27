// Created by Ben on 18/03/2022

#pragma once

#include "ProjectileBase.hpp"

class KnockOutDart : public ProjectileBase
{
 public:
  KnockOutDart()           = default;
  ~KnockOutDart() override = default;

  void start() override;

 protected:
  void onCollisionEnter(bcw::Collider* collision) override;
  void onCollisionExit(bcw::Collider* collision) override;

 private:
  const double KNOCK_OUT_TIME = 10.0;
};
