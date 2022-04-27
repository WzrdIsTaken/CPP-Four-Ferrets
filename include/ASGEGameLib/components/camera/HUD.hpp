//
// Created by Norbert on 10/03/2022.
//

#ifndef ASGEGAME_HUD_HPP
#define ASGEGAME_HUD_HPP

#include "../../Component.hpp"
#include <utility>

class HUD : public bcw::Component
{
 public:
  HUD(ASGE::Point2D pos, const ASGE::Font* font_face);

  void update(const ASGE::GameTime& game_time) override;
  void render();
  void setPosition(ASGE::Point2D position);
  void setID(uint16_t id);

  void setPlayerText(const std::string& text);
  void setHealthText(const std::string& text);
  void setAbilityText(const std::string& text);

 private:
  uint16_t hud_id;
  ASGE::Point2D hud_position{ 0.0F, 0.0F };

  ASGE::Text player_text;
  ASGE::Point2D player_text_pos{ 55.0F, 45.0F };
  ASGE::Text health_text;
  ASGE::Point2D health_text_pos{ 55.0F, 535.0F };
  ASGE::Text cool_down_text;
  ASGE::Point2D cool_down_text_pos{ 620.0F, 535.0F };

  void initText(const ASGE::Font* font_face);
};

#endif // ASGEGAME_HUD_HPP
