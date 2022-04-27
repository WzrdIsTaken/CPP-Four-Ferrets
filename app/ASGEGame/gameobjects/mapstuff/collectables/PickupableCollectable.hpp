// Created by Ben on 24/03/2022

#pragma once

#include "CollectableBase.hpp"

class PickupableCollectable : public CollectableBase
{
 public:
  PickupableCollectable()           = default;
  ~PickupableCollectable() override = default;

  void start() override;
  void render(const ASGE::GameTime& game_time) override;

  virtual void onUseBegin(const bcw::Vector2& use_direction) = 0;
  virtual void onUseEnd(const bcw::Vector2& use_direction)   = 0;

  // For when the object is picked up, and we want to control its movement
  void forceMove(const ASGE::GameTime& game_time, const bcw::Vector2& position);
  // For when the object is picked up, and we want to control its rotation
  void forceRotate(const ASGE::GameTime& game_time, const bcw::Vector2& rotation);
  // For when the object is picked up, and we want to control its rendering
  void forceRender(const ASGE::GameTime& game_time);

  // Returns the map objects sprite, needed for some cool ui
  ASGE::Sprite const* getIconSprite() const;

  // Needed for idk dude its just needed ok
  void setAttachedToScene(const std::shared_ptr<bcw::Scene>& _attached_to_scene);

  [[maybe_unused]] [[nodiscard]] bool getIsPickedUp() const;
  void setIsPickedUp(bool _is_picked_up);

 protected:
  void onCollisionEnter(bcw::Collider* collision) override;
  void onCollisionExit(bcw::Collider* collision) override;

  void removeCollectableFromPlayersInventory();

  bool is_picked_up             = false;
  bcw::Vector2 picked_up_offset = bcw::Vector2(0, 0);

  std::unique_ptr<ASGE::Sprite> icon_sprite = nullptr;

 private:
  std::shared_ptr<bcw::Scene> attached_to_scene = nullptr;
};
