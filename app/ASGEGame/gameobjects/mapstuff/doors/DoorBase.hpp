// Created by Ben on 16/03/2022

#pragma once

#include "../MapObject.hpp"

class DoorBase : public MapObject
{
 public:
  DoorBase()           = default;
  ~DoorBase() override = default;

  void start() override;
  void init(
    const std::string& sprite_file_path, const bcw::Vector2& position,
    const bcw::Vector2& open_position, bool start_open, bool start_locked);
  void update(const ASGE::GameTime& game_time) override;

 protected:
  void toggleDoor();
  void toggleUnlock(float _unlock_time);
  bool checkLockedState() const;

  bcw::Vector2 start_location;
  bcw::Vector2 end_location;
  float speed = 50.0F;

  bool locked = false;

 private:
  // -> c <-> o <-> l <-> l <-> i <-> s <-> i <-> o <-> n <-
  void onCollisionEnter(bcw::Collider* collision);
  void onCollisionExit(bcw::Collider* collision);

  std::unique_ptr<bcw::Collider> collider = nullptr;

  // Opening / Closing
  bcw::Vector2 target_location;
  bool open   = false;
  bool moving = false;

  // Locking / Unlocking
  bool unlocking        = false;
  float unlock_time     = 3.0F;
  float unlock_progress = 0.0F;
};
