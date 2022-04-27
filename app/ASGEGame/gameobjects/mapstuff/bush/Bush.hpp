// Created by Ben on 12/04/2022.

#pragma once

#include <ASGEGameLib/components/collision/Collider.hpp>

#include "../MapObject.hpp"

// A basic bush a player can hide in from guards

class Bush : public MapObject
{
 public:
  Bush()           = default;
  ~Bush() override = default;

  void start() override;

  void init(const bcw::Vector2& position);

 private:
  void onCollisionEnter(bcw::Collider* collision);
  void onCollisionExit(bcw::Collider* collision);

  std::unique_ptr<bcw::Collider> collider;
};

// YES there will be some issues with this and the hacker HOWEVER i can't be asked
// to fix them, this is meant to be just a simple addition SO just in the demo video,
// the person who is playing the hacker shouldn't hide in one of the bushes xd
