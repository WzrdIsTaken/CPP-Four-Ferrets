//
// Created by Norbert on 14/04/2022.
//

#include "../../../../include/ASGEGameLib/components/player/NamePicker.hpp"

using driver = bcw::Driver;

/// ----------------------------- PUBLIC FUNCTIONS ------------------------------

NamePicker::NamePicker(const ASGE::Font* font_face, ASGE::Colour colour)
{
  initText(font_face, colour);
}

void NamePicker::update(const ASGE::GameTime& game_time)
{
  Component::update(game_time);
}

void NamePicker::render(const ASGE::GameTime& /*game_time*/)
{
  driver::getRenderer()->render(*picker_bg);
  driver::getRenderer()->render(output_text);
}

void NamePicker::onKey(const ASGE::KeyEvent* key_event)
{
  if (key_event->action == 1)
  {
    static bool capital = true;
    const auto PRESSED  = key_event->key;
    if (
      ((ASGE::KEYS::KEY_A <= PRESSED && PRESSED <= ASGE::KEYS::KEY_Z) ||
       PRESSED == ASGE::KEYS::KEY_SPACE) &&
      input_vector.size() < MAX_INPUT_LENGTH)
    {
      driver::getAudioHandler()->playAudio("data/audio/menu_select.mp3");
      if (capital || PRESSED == ASGE::KEYS::KEY_SPACE)
      {
        input_vector.emplace_back(PRESSED);
        PRESSED == ASGE::KEYS::KEY_SPACE ? capital = true : capital = false;
      }
      else
      {
        input_vector.emplace_back(PRESSED + 32);
        capital = false;
      }
      output_text.setString(updateString());
    }
    else if (PRESSED == ASGE::KEYS::KEY_BACKSPACE && !input_vector.empty())
    {
      (ASGE::KEYS::KEY_A <= input_vector.back() && input_vector.back() <= ASGE::KEYS::KEY_Z) ||
          input_vector.size() == 1
        ? capital = true
        : capital = false;
      input_vector.pop_back();
      output_text.setString(updateString());
      //      input_vector.empty() ? capital = true : capital = false;
    }
  }
  output_text.setPositionX(TEXT_POS.x - output_text.getWidth() * 0.5F);
}

void NamePicker::setActiveState(bool state)
{
  input_active = state;
}

bool NamePicker::isActive() const
{
  return input_active;
}

std::string NamePicker::getGangName()
{
  return output_text.getString();
}

/// ----------------------------- PRIVATE FUNCTIONS ------------------------------

void NamePicker::initText(const ASGE::Font* font_face, ASGE::Colour colour)
{
  picker_bg = driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    picker_bg.get(), "data/images/gang_name_picker_bg.png", bcw::Vector2(0.0F, 0.0F), 1.0F);
  picker_bg->xPos(TEXT_POS.x - picker_bg->width() * 0.5F);
  picker_bg->yPos(TEXT_POS.y - picker_bg->height() * 0.65F);
  picker_bg->setGlobalZOrder(4);

  output_text.setFont(*font_face)
    .setColour(colour)
    .setString("")
    .setPosition(TEXT_POS)
    .setZOrder(5)
    .setScale(0.6F);
}

std::string NamePicker::updateString()
{
  std::string updated_str;
  for (auto& ch : input_vector)
  {
    updated_str += ch;
  }
  return updated_str;
}
