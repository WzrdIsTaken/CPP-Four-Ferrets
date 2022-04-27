//
// Created by Norbert on 17/03/2022.
//

#include "../../../../include/ASGEGameLib/components/player/CharacterPicker.hpp"

/// ----------------------------- PUBLIC FUNCTIONS ------------------------------
CharacterPicker::CharacterPicker(ASGE::Point2D pos, const ASGE::Font* font_face) :
  start_pos(std::move(pos))
{
  initOptions(font_face);
  initClassIcons();
  changeColour(0, selected_colour);
}

void CharacterPicker::moveSelector(int value)
{
  changeColour(static_cast<uint16_t>(current_pick), base_colour);
  current_pick += value;
  if (current_pick < 0)
  {
    current_pick = static_cast<int>(options.size() - 1);
  }
  else if (current_pick >= static_cast<int>(options.size()))
  {
    current_pick = 0;
  }

  checkAvailability(value);

  changeColour(static_cast<uint16_t>(current_pick), selected_colour);
}

void CharacterPicker::updateList(uint16_t value)
{
  unavailable_classes.push_back(value);
  changeColour(static_cast<uint16_t>(current_pick), base_colour);
  checkAvailability(1);
  changeColour(static_cast<uint16_t>(current_pick), selected_colour);
  options[value].setColour(unavailable_colour);
}

int CharacterPicker::selectCharacter()
{
  selected_class.setString(options[static_cast<uint64_t>(current_pick)].getString());
  lock();
  return current_pick;
}

bool CharacterPicker::isLocked() const
{
  return picker_locked;
}

void CharacterPicker::render()
{
  if (!picker_locked)
  {
    for (auto& option : options)
    {
      driver::getRenderer()->render(option);
    }
  }
  else
  {
    driver::getRenderer()->render(selected_class);
  }
  driver::getRenderer()->render(*class_icons[static_cast<uint64_t>(current_pick)]);
}

/// ----------------------------- PRIVATE FUNCTIONS -----------------------------
void CharacterPicker::initOptions(const ASGE::Font* font_face)
{
  uint16_t ctr = 0; // loop counter
  for (auto& option : classes)
  {
    auto& text = options.emplace_back();
    text.setFont(*font_face)
      .setColour(base_colour)
      .setString(option)
      .setPosition(ASGE::Point2D{ start_pos.x, start_pos.y + float(ctr) * 40.0F })
      .setZOrder(10)
      .setScale(0.4F);
    ++ctr;
  }
  selected_class.setFont(*font_face)
    .setColour(ASGE::COLOURS::GREEN)
    .setString("SELECTED CLASS")
    .setPosition(start_pos)
    .setZOrder(10);
}

void CharacterPicker::changeColour(uint16_t idx, ASGE::Colour colour)
{
  options[idx].setColour(colour);
}

void CharacterPicker::checkAvailability(int value)
{
  while (std::find(unavailable_classes.begin(), unavailable_classes.end(), current_pick) !=
         unavailable_classes.end())
  {
    current_pick += value;
    if (current_pick < 0)
    {
      current_pick = static_cast<int>(options.size() - 1);
    }
    else if (current_pick > static_cast<int>(options.size() - 1))
    {
      current_pick = 0;
    }
  }
}

void CharacterPicker::lock()
{
  picker_locked = true;
}

void CharacterPicker::initClassIcons()
{
  auto& thief_spr = class_icons.emplace_back(driver::getRenderer()->createUniqueSprite());
  bcw::Driver::getRenderHandler()->setupSprite(
    thief_spr.get(), "data/images/thief/thief_idle.png", bcw::Vector2(start_pos.x + 250.0F, start_pos.y), 8.0F);
  thief_spr->setGlobalZOrder(2);

  auto& hacker_spr = class_icons.emplace_back(driver::getRenderer()->createUniqueSprite());
  bcw::Driver::getRenderHandler()->setupSprite(
    hacker_spr.get(), "data/images/hacker/hacker_idle.png", bcw::Vector2(start_pos.x + 250.0F, start_pos.y), 8.0F);
  hacker_spr->setGlobalZOrder(2);

  auto& bruiser_spr = class_icons.emplace_back(driver::getRenderer()->createUniqueSprite());
  bcw::Driver::getRenderHandler()->setupSprite(
    bruiser_spr.get(), "data/images/bruiser/bruiser_idle.png", bcw::Vector2(start_pos.x + 250.0F, start_pos.y), 8.0F);
  bruiser_spr->setGlobalZOrder(2);

  auto& marksman_spr = class_icons.emplace_back(driver::getRenderer()->createUniqueSprite());
  bcw::Driver::getRenderHandler()->setupSprite(
    marksman_spr.get(), "data/images/marksman/marksman_idle.png", bcw::Vector2(start_pos.x + 250.0F, start_pos.y), 8.0F);
  marksman_spr->setGlobalZOrder(2);
}
