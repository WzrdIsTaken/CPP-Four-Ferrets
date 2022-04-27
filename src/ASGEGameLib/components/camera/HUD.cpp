//
// Created by Norbert on 10/03/2022.
//

#include "../../../../include/ASGEGameLib/components/camera/HUD.hpp"

using driver = bcw::Driver;

HUD::HUD(ASGE::Point2D pos, const ASGE::Font* font_face) : hud_position(std::move(pos))
{
  initText(font_face);
}

void HUD::update(const ASGE::GameTime& /*game_time*/)
{
  player_text.setPosition(
    ASGE::Point2D{ hud_position.x + player_text_pos.x, hud_position.y + player_text_pos.y });
  health_text.setPosition(
    ASGE::Point2D{ hud_position.x + health_text_pos.x, hud_position.y + health_text_pos.y });
  cool_down_text.setPosition(
    ASGE::Point2D{ hud_position.x + cool_down_text_pos.x, hud_position.y + cool_down_text_pos.y });
}

void HUD::render()
{
  driver::getRenderer()->render(player_text);
  driver::getRenderer()->render(health_text);
  driver::getRenderer()->render(cool_down_text);
}

void HUD::setPosition(ASGE::Point2D position)
{
  hud_position = position;
}

void HUD::setID(uint16_t id)
{
  hud_id = id;
  player_text.setString("Player " + std::to_string(hud_id + 1));
}

void HUD::initText(const ASGE::Font* font_face)
{
  player_text.setFont(*font_face)
    .setColour(ASGE::COLOURS::GHOSTWHITE)
    .setString("Player #")
    .setScale(0.45F)
    .setPosition(
      ASGE::Point2D{ hud_position.x + player_text_pos.x, hud_position.y + player_text_pos.y })
    .setZOrder(20);

  health_text.setFont(*font_face)
    .setColour(ASGE::COLOURS::GHOSTWHITE)
    .setString("Health 100%")
    .setScale(0.45F)
    .setPosition(
      ASGE::Point2D{ hud_position.x + health_text_pos.x, hud_position.y + health_text_pos.y })
    .setZOrder(20);

  cool_down_text.setFont(*font_face)
    .setColour(ASGE::COLOURS::GHOSTWHITE)
    .setString("ABILITY ON COOLDOWN")
    .setScale(0.45F)
    .setPosition(
      ASGE::Point2D{ hud_position.x + cool_down_text_pos.x, hud_position.y + cool_down_text_pos.y })
    .setZOrder(20);
}

void HUD::setPlayerText(const std::string& text)
{
  player_text.setString(text);
}

void HUD::setHealthText(const std::string& text)
{
  health_text.setString(text);
}

void HUD::setAbilityText(const std::string& text)
{
  cool_down_text.setString(text);
}
