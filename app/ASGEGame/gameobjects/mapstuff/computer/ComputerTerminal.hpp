// Created by Ben on 06/04/2022

#pragma once

#include "../IHackable.hpp"
#include "../IInteractable.hpp"

#include "../MapObject.hpp"

class ComputerTerminal
  : public MapObject
  , public IInteractable
  , public IHackable
{
 public:
  ComputerTerminal()           = default;
  ~ComputerTerminal() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;

  void
  init(const bcw::Vector2& position, const std::vector<std::shared_ptr<IHackable>>& _linked_objects);

  void onInteractionBegin() override;
  void onInteractionEnd() override;

  void onHackBegin(float _hack_speed) override;
  void onHackEnd() override;
  bool getHackedState() override;

 private:
  void onCollisionEnter(bcw::Collider* collision);
  void onCollisionExit(bcw::Collider* collision);

  // this is kinda scuffed but deadline kinda close so will just deal with it
  std::vector<std::shared_ptr<IHackable>> linked_objects;
  std::unique_ptr<bcw::Collider> collider = nullptr;

  bool hacking        = false;
  bool hacked         = false;
  float hack_time     = 0.0F;
  float hack_progress = 0.0F;
};
