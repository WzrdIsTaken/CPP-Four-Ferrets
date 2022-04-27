//
// Created by Norbert on 03/03/2022.
//

#include "../../../../include/ASGEGameLib/components/gamepad/GamePad.hpp"

GamePad::GamePad(ASGE::Input* asge_input) : inputs(*asge_input)
{
  game_pads = inputs.getGamePads();
}

void GamePad::update(const ASGE::GameTime& /*game_time*/)
{
  game_pads = inputs.getGamePads();
  //  for (auto& gamepad : inputs.getGamePads())
  //  {
  //    gamepad_state.insert_or_assign(gamepad.idx, gamepad);
  //  }
}

ASGE::Point2D GamePad::leftStickState(uint16_t game_pad_idx) const
{
  if (checkStickDeadzone(game_pad_idx))
  {
    return { inputs.getGamePad(game_pad_idx).axis[0], inputs.getGamePad(game_pad_idx).axis[1] };
  }
  return ASGE::Point2D{ 0.0F, 0.0F };
}

bool GamePad::buttonA(uint16_t game_pad_idx)
{
  return checkButton(game_pad_idx, 0);
}

bool GamePad::buttonB(uint16_t game_pad_idx)
{
  return checkButton(game_pad_idx, 1);
}

bool GamePad::buttonX(uint16_t game_pad_idx)
{
  return checkButton(game_pad_idx, 2);
}

bool GamePad::buttonY(uint16_t game_pad_idx)
{
  return checkButton(game_pad_idx, 3);
}
bool GamePad::bumperLeft(uint16_t game_pad_idx)
{
  return checkButton(game_pad_idx, 4);
}
bool GamePad::bumperRight(uint16_t game_pad_idx)
{
  return checkButton(game_pad_idx, 5);
}
bool GamePad::buttonBack(uint16_t game_pad_idx)
{
  return checkButton(game_pad_idx, 6);
}
bool GamePad::buttonStart(uint16_t game_pad_idx)
{
  return checkButton(game_pad_idx, 7);
}
bool GamePad::dPad_UP(uint16_t game_pad_idx)
{
  return checkButton(game_pad_idx, 11);
}
bool GamePad::dPad_RIGHT(uint16_t game_pad_idx)
{
  return checkButton(game_pad_idx, 12);
}
bool GamePad::dPad_DOWN(uint16_t game_pad_idx)
{
  return checkButton(game_pad_idx, 13);
}
bool GamePad::dPad_LEFT(uint16_t game_pad_idx)
{
  return checkButton(game_pad_idx, 14);
}

bool GamePad::checkStickDeadzone(uint16_t game_pad_idx) const
{
  return sqrt(
           pow(inputs.getGamePad(game_pad_idx).axis[0], 2) +
           pow(inputs.getGamePad(game_pad_idx).axis[1], 2)) > stick_tolerance;
}

bool GamePad::checkButton(uint16_t game_pad_idx, uint16_t button_idx)
{
  return inputs.getGamePad(game_pad_idx).buttons[button_idx] != 0U &&
         game_pads[game_pad_idx].buttons[button_idx] == 0U;
}
