//
// Created by Norbert on 17/03/2022.
//

#ifndef ASGEGAME_CHARACTERPICKER_HPP
#define ASGEGAME_CHARACTERPICKER_HPP

#include "../../Component.hpp"
#include <utility>

using driver = bcw::Driver;

class CharacterPicker : public bcw::Component
{
 public:
  CharacterPicker(ASGE::Point2D pos, const ASGE::Font* font_face);
  ~CharacterPicker() override = default;

  void moveSelector(int value);
  int selectCharacter();
  void updateList(uint16_t value);

  [[nodiscard]] bool isLocked() const;

  void render();

 private:
  void initOptions(const ASGE::Font* font_face);
  void changeColour(uint16_t idx, ASGE::Colour colour);
  void checkAvailability(int value);
  void lock();
  void initClassIcons();

  bool picker_locked = false;
  int current_pick   = 0;
  ASGE::Text selected_class;
  std::vector<ASGE::Text> options;
  std::vector<std::string> classes{ "Thief", "Hacker", "Bruiser", "Marksman" };
  std::vector<std::unique_ptr<ASGE::Sprite>> class_icons;
  std::list<uint16_t> unavailable_classes;
  ASGE::Colour base_colour        = ASGE::COLOURS::BLACK;
  ASGE::Colour selected_colour    = ASGE::COLOURS::ORANGE;
  ASGE::Colour unavailable_colour = ASGE::COLOURS::RED;
  ASGE::Point2D start_pos{ 0.0F, 0.0F };
};

#endif // ASGEGAME_CHARACTERPICKER_HPP
