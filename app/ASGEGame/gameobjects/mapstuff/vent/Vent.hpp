//
// Created by teobo on 14/04/2022.
//

#ifndef ASGEGAME_VENT_HPP
#define ASGEGAME_VENT_HPP

#include <ASGEGameLib/components/collision/Collider.hpp>

#include "../MapObject.hpp"

class Vent : public MapObject
{
 public:
  Vent()           = default;
  ~Vent() override = default;

  void start() override;

  void init(const bcw::Vector2& start_position, const bcw::Vector2& _vent_position);

 private:
  // Have a pointer to the collider
  std::unique_ptr<bcw::Collider> collider = nullptr;

  // Just like Unity!
  void onCollisionEnter(bcw::Collider* collision);
  void onCollisionExit(bcw::Collider* collision);

  // The logic
  bcw::Vector2 vent_position{};
};

#endif // ASGEGAME_VENT_HPP
