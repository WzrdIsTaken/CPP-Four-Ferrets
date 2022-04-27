// Created by Ben on 26/02/2022

#include <utility> // because clang said so...

#include "../../../../include/ASGEGameLib/components/ui/GamePadInputUI.hpp"

bcw::GamePadInputUI::GamePadInputUI(
  std::function<void(ScrollDirection)> _on_scroll_callback, const AxisSettings& axis_settings,
  std::function<void(int)> _on_button_press_callback, const ButtonSettings& button_settings,
  const bool _only_accept_master_controller) // NOLINT
  :
  on_scroll_callback(std::move(_on_scroll_callback)),
  on_button_press_callback(std::move(_on_button_press_callback)),

  detection_axes(axis_settings.detection_axes), scroll_value(axis_settings.scroll_value),
  reset_value(axis_settings.reset_value),

  only_accept_master_controller(_only_accept_master_controller)
{
  for (const int button : button_settings.buttons) // NOLINT
  {
    buttons[button] = true;
  }
}

void bcw::GamePadInputUI::pollInput(const ASGE::GamePadData game_pad_input)
{
  if (
    only_accept_master_controller &&
    !bcw::Driver::getInputHandler()->checkIfControllerHasPriority(game_pad_input))
  {
    return;
  }

  pollAxes(game_pad_input);
  pollButtons(game_pad_input);
}

void bcw::GamePadInputUI::pollAxes(const ASGE::GamePadData game_pad_input)
{
  bcw::Vector2 axis_state =
    bcw::Vector2(game_pad_input.axis[detection_axes.x], game_pad_input.axis[detection_axes.y]);

  if (axis_state.y > scroll_value && scroll_direction == ScrollDirection::NONE)
  {
    scroll_direction = ScrollDirection::UP;
    on_scroll_callback(scroll_direction);
  }
  else if (axis_state.y < -scroll_value && scroll_direction == ScrollDirection::NONE)
  {
    scroll_direction = ScrollDirection::DOWN;
    on_scroll_callback(scroll_direction);
  }
  else if (axis_state.x > scroll_value && scroll_direction == ScrollDirection::NONE)
  {
    scroll_direction = ScrollDirection::RIGHT;
    on_scroll_callback(scroll_direction);
  }
  else if (axis_state.x < -scroll_value && scroll_direction == ScrollDirection::NONE)
  {
    scroll_direction = ScrollDirection::LEFT;
    on_scroll_callback(scroll_direction);
  }
  else if (
    axis_state.y < reset_value && axis_state.y > -reset_value && axis_state.x < reset_value &&
    axis_state.x > -reset_value && scroll_direction != ScrollDirection::NONE)
  {
    scroll_direction = ScrollDirection::NONE;
    on_scroll_callback(scroll_direction);
  }
}

void bcw::GamePadInputUI::pollButtons(const ASGE::GamePadData game_pad_input)
{
  for (const auto& button_pair : buttons)
  {
    if (game_pad_input.buttons[button_pair.first] != 0U && !button_pair.second)
    {
      on_button_press_callback(button_pair.first);
      buttons[button_pair.first] = true;
    }
    else if (game_pad_input.buttons[button_pair.first] == 0U && button_pair.second)
    {
      buttons[button_pair.first] = false;
    }
  }
}

bcw::Vector2Int bcw::GamePadInputUI::getDetectionAxes() const
{
  return detection_axes;
}

void bcw::GamePadInputUI::setDetectionAxes(const bcw::Vector2Int& _detection_axes) // NOLINT
{
  if (detection_axes.x != ASGE::GAMEPAD::AXIS_LEFT_X || detection_axes.x != ASGE::GAMEPAD::AXIS_RIGHT_X)
  {
    throw std::invalid_argument(
      "The x detection axis " + std::to_string(detection_axes.x) + " is not valid!");
  }

  if (detection_axes.y != ASGE::GAMEPAD::AXIS_LEFT_Y || detection_axes.y != ASGE::GAMEPAD::AXIS_RIGHT_Y)
  {
    throw std::invalid_argument(
      "The y detection axis " + std::to_string(detection_axes.y) + " is not valid!");
  }

  detection_axes = _detection_axes;
}

float bcw::GamePadInputUI::getScrollValue() const
{
  return scroll_value;
}

void bcw::GamePadInputUI::setScrollValue(const float _scroll_value) // NOLINT
{
  scroll_value = std::clamp(_scroll_value, reset_value + V, 1.0F);
}

float bcw::GamePadInputUI::getResetValue() const
{
  return reset_value;
}

void bcw::GamePadInputUI::setResetValue(const float _reset_value) // NOLINT
{
  reset_value = std::clamp(_reset_value, 0.0F, scroll_value - V);
}

bool bcw::GamePadInputUI::getOnlyAcceptMasterController() const
{
  return only_accept_master_controller;
}

void bcw::GamePadInputUI::setOnlyAcceptMasterController(
  const bool _only_accept_master_controller) // NOLINT
{
  only_accept_master_controller = _only_accept_master_controller;
}
