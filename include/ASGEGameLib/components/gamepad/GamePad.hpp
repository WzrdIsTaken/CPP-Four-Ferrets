//
// Created by Norbert on 03/03/2022.
//

#ifndef ASGEGAME_GAMEPAD_H
#define ASGEGAME_GAMEPAD_H

#include "../../Component.hpp"
#include <Engine/Logger.hpp>
#include <cmath>

class GamePad : public bcw::Component
{
 public:
  explicit GamePad(ASGE::Input* asge_input);
  ~GamePad() override = default;

  /// To store the state of all the gamepads at the end of the frame
  void update(const ASGE::GameTime& game_time) override;

  /// Functions to fetch gamepad data
  [[nodiscard]] ASGE::Point2D leftStickState(uint16_t game_pad_idx) const;
  [[nodiscard]] bool buttonA(uint16_t game_pad_idx);
  [[nodiscard]] bool buttonB(uint16_t game_pad_idx);
  [[nodiscard]] bool buttonX(uint16_t game_pad_idx);
  [[nodiscard]] bool buttonY(uint16_t game_pad_idx);
  [[nodiscard]] bool bumperLeft(uint16_t game_pad_idx);
  [[nodiscard]] bool bumperRight(uint16_t game_pad_idx);
  [[nodiscard]] bool buttonBack(uint16_t game_pad_idx);
  [[nodiscard]] bool buttonStart(uint16_t game_pad_idx);
  [[nodiscard]] bool dPad_UP(uint16_t game_pad_idx);
  [[nodiscard]] bool dPad_RIGHT(uint16_t game_pad_idx);
  [[nodiscard]] bool dPad_DOWN(uint16_t game_pad_idx);
  [[nodiscard]] bool dPad_LEFT(uint16_t game_pad_idx);

 private:
  float stick_tolerance = 0.1F;
  ASGE::Input& inputs;
  //  std::map<int, ASGE::GamePadData> gamepad_state{};
  std::vector<ASGE::GamePadData> game_pads;

  /// Checks whether the stick input vector is higher than the tolerance,
  /// returns TRUE when the input is higher than the that.
  [[nodiscard]] bool checkStickDeadzone(uint16_t game_pad_idx) const;

  /// Simple "is_pressed" like check for the buttons
  bool checkButton(uint16_t game_pad_idx, uint16_t button_idx);
};

#endif // ASGEGAME_GAMEPAD_H
