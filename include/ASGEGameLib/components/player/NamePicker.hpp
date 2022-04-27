//
// Created by Norbert on 14/04/2022.
//

#ifndef ASGEGAME_NAMEPICKER_HPP
#define ASGEGAME_NAMEPICKER_HPP

#include "../../Component.hpp"

class NamePicker : public bcw::Component
{
 public:
  NamePicker(const ASGE::Font* font_face, ASGE::Colour colour);
  ~NamePicker() override = default;

  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time);

  void onKey(const ASGE::KeyEvent* key_event);

  void setActiveState(bool state);
  [[nodiscard]] bool isActive() const;
  std::string getGangName();

 private:
  std::vector<char> input_vector;
  const uint16_t MAX_INPUT_LENGTH = 28;
  const ASGE::Point2D TEXT_POS{ 960.0F, 650.0F };

  std::unique_ptr<ASGE::Sprite> picker_bg;
  ASGE::Text output_text;
  bool input_active = false;

  void initText(const ASGE::Font* font_face, ASGE::Colour colour);
  std::string updateString();
};

#endif // ASGEGAME_NAMEPICKER_HPP
