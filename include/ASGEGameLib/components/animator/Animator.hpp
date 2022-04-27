// Created by Ben on 04/02/2022

// A basic animator component

#pragma once

#include <Engine/Sprite.hpp>

#include <map>
#include <vector>

#include "../../Component.hpp"

namespace bcw
{
  class [[maybe_unused]] Animator : public bcw::Component
  {
   public:
    // For individual sprites
    [[maybe_unused]] Animator(
      ASGE::Sprite* _sprite, const std::map<std::string, std::vector<std::string>>& _animations,
      float _tick_speed, bool _loadingResourceFromDifferentThreadDumbFix = false);
    // For sprite sheets
    [[maybe_unused]] Animator(
      ASGE::Sprite* _sprite,
      const std::map<std::string, std::vector<std::array<float, 4>>>& _animations,
      float _tick_speed, bool _loadingResourceFromDifferentThreadDumbFix = false);
    ~Animator() override = default;

    // a n i m a t e
    void update(const ASGE::GameTime& game_time) override;

    // Returns the tick speed of the animation
    [[maybe_unused]] [[nodiscard]] float getTickSpeed() const;
    // Sets the tick speed of the animation
    [[maybe_unused]] void setTickSpeed(float new_speed);

    // Returns the current animation's name
    [[maybe_unused]] [[nodiscard]] std::string getAnimation() const;
    // Sets the animation via its string name.
    // Returns true if the current animation is already new_animation or if the switch was
    // successful. Returns false if new_animation doesn't exist
    [[maybe_unused]] bool setAnimation(const std::string& new_animation);

    // Returns the current frame of the animation
    [[maybe_unused]] [[nodiscard]] unsigned long getCurrentFrame() const;
    // Sets the animation's current frame
    [[maybe_unused]] bool setCurrentFrame(unsigned long frame);

   private:
    enum AnimationMode
    {
      INDIVIDUAL,
      SPRITE_SHEET
    };
    const AnimationMode ANIMATION_MODE;

    ASGE::Sprite* sprite;
    std::map<std::string, std::vector<std::string>> individual_rendering_animations;
    std::map<std::string, std::vector<std::array<float, 4>>> sprite_sheet_rendering_animations;

    float tick_speed;
    float clock;

    unsigned long current_frame;
    std::string current_animation;

    // Removes the first frame from the animation, because if an animation (therefore sprite)
    // is loaded from a different thread then the image is corrupted. But because the rest of the
    // sprites will be loaded inside the animator (on the main thread) it's all good.
    // If you actually have a good structure for communicating between threads, then this shouldn't
    // be a problem...
    bool loadingResourceFromDifferentThreadDumbFix = false;
  };
} // namespace bcw
