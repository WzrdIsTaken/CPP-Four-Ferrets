// Created by Ben on 26/02/2022

#include "ASGEGameLib/Driver.hpp"

class LevelSelectScene
  : public bcw::Scene
  , public bcw::IClickHandler
{
 public:
  LevelSelectScene()           = default;
  ~LevelSelectScene() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;
  void onMouse(const ASGE::ClickEvent* mouse_event) override;

 private:
  void initSprites();
  void onSelect(int button);

  void pressButton(const ASGE::Text* text);
  void updateGamePad();

  void initText();

  std::shared_ptr<GameData> game_data;
  std::unique_ptr<ASGE::Sprite> background;

  ASGE::Text title_text;

  ASGE::Text level1_text;
  ASGE::Text level2_text;
  ASGE::Text level3_text;

  std::vector<ASGE::Text*> level_options;
  ASGE::Text* currently_selected_option = nullptr;

  const int SELECT_BUTTON = ASGE::GAMEPAD::BUTTON_A;

  bool loading_next_scene = false;
  std::unique_ptr<ASGE::Sprite> loading_screen;

  ASGE::Colour selected_colour = ASGE::COLOURS::ORANGE;
  ASGE::Colour normal_colour   = ASGE::COLOURS::WHITE;
};
