// Created by Ben on 10/03/2022

#pragma once

#include <ASGEGameLib/components/camera/Camera.hpp>

#include "CharacterBase.hpp"

class PlayerCharacter
  : public CharacterBase
  , public bcw::IKeyHandler
  , public bcw::IClickHandler
{
 public:
  PlayerCharacter()           = default;
  ~PlayerCharacter() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;
  void onKey(const ASGE::KeyEvent* key_event) override;
  void onMouse(const ASGE::ClickEvent* mouse_event) override;

  void init(const bcw::Vector2& spawn_position);

  void setUpClamp(ASGE::Camera::CameraView clamp_value);
  void setCameraStartPos(ASGE::Point2D start_pos);
  void setPlayerPos(ASGE::Point2D pos);
  void setID(uint16_t id);
  float getSpeed();
  void movePlayer(ASGE::Point2D vector, const ASGE::GameTime& game_time);
  void rotatePlayer(float left_stick_x, float left_stick_y);
  Camera* getCamera();

  void heal(int heal) override;
  void takeDamage(int damage) override;

  enum class CharacterClasses
  {
    THIEF    = 0,
    HACKER   = 1,
    BRUISER  = 2,
    MARKSMAN = 3
  };

  float radius = 0;

 protected:
  void onDie() override;
  void onRevive() override;

  std::unique_ptr<Camera> camera;

  CharacterClasses character_class = CharacterClasses::THIEF;

 private:
  std::string calculateAndFormatHealth();
  std::string calculateAndFormatAbilityCoolDown();

  [[maybe_unused]] uint16_t player_id = 0;
  std::map<int, bool> key_map{};

  const int MOVE_UP_KEY         = ASGE::KEYS::KEY_W;
  const int MOVE_DOWN_KEY       = ASGE::KEYS::KEY_S;
  const int MOVE_LEFT_KEY       = ASGE::KEYS::KEY_A;
  const int MOVE_RIGHT_KEY      = ASGE::KEYS::KEY_D;
  const int ABILITY_KEY         = ASGE::KEYS::KEY_Q;
  const int INTERACT_KEY        = ASGE::KEYS::KEY_E;
  const int SCROLL_INV_UP_KEY   = ASGE::KEYS::KEY_R;
  const int SCROLL_INV_DOWN_KEY = ASGE::KEYS::KEY_F;
};
