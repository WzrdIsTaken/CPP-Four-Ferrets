// Created by Ben on 26/02/2022

#pragma once

#include <functional>

#include "../../Component.hpp"

// Basic class to make some simple ui / menu scenes easier to create with a game pad

namespace bcw
{
  class [[maybe_unused]] GamePadInputUI : public bcw::Component
  {
   public:
    enum ScrollDirection
    {
      UP,
      DOWN,
      LEFT,
      RIGHT,
      NONE
    };

    struct AxisSettings
    {
      bcw::Vector2Int detection_axes =
        bcw::Vector2Int(ASGE::GAMEPAD::AXIS_LEFT_X, ASGE::GAMEPAD::AXIS_LEFT_Y);
      float scroll_value = 0.5F;
      float reset_value  = 0.25F;
    };

    struct ButtonSettings
    {
      std::vector<int> buttons = std::vector<int>{ ASGE::GAMEPAD::BUTTON_B };
    };

    GamePadInputUI(
      std::function<void(ScrollDirection)> _on_scroll_callback, const AxisSettings& axis_settings,
      std::function<void(int)> _on_button_press_callback, const ButtonSettings& button_settings,
      bool _only_accept_master_controller = true);
    ~GamePadInputUI() override = default;

    // Polls for valid input
    [[maybe_unused]] void pollInput(ASGE::GamePadData game_pad_input);

    // Returns detection_axis
    [[maybe_unused]] [[nodiscard]] bcw::Vector2Int getDetectionAxes() const;
    // Sets detection_axis, making sure that it's actually an axis
    [[maybe_unused]] void setDetectionAxes(const bcw::Vector2Int& _detection_axes);

    // Returns scroll_value
    [[maybe_unused]] [[nodiscard]] float getScrollValue() const;
    // Sets scroll_value, capping it at between 1 and reset_value + 0.1F
    [[maybe_unused]] void setScrollValue(float _scroll_value);

    // Returns reset_value
    [[maybe_unused]] [[nodiscard]] float getResetValue() const;
    // Sets reset_value, capping it between scroll_value - 0.1F and 0
    [[maybe_unused]] void setResetValue(float _reset_value);

    // Returns only_accept_master_controller
    [[maybe_unused]] [[nodiscard]] bool getOnlyAcceptMasterController() const;
    // Sets only_accept_master_controller
    [[maybe_unused]] void setOnlyAcceptMasterController(bool _only_accept_master_controller);

   private:
    void pollAxes(ASGE::GamePadData game_pad_input);
    void pollButtons(ASGE::GamePadData game_pad_input);

    std::function<void(ScrollDirection)> on_scroll_callback;
    std::function<void(int)> on_button_press_callback;

    // Scroll variables
    ScrollDirection scroll_direction = ScrollDirection::NONE;
    bcw::Vector2Int detection_axes;

    float scroll_value;
    float reset_value;

    const float V = 0.1F; // I can't remember its proper name rn!!!

    // Button variables
    std::map<int, bool> buttons;

    // Should input only be accepted from the master controller?
    bool only_accept_master_controller;
  };
} // namespace bcw