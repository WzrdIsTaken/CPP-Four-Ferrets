// Created by Ben on 24/02/2022

#pragma once

#include <vector>

#include "ASGEGameLib/Driver.hpp"
#include "ASGEGameLib/components/player/NamePicker.hpp"

#include "GameData.hpp"

class MainMenuScene
  : public bcw::Scene
  , public bcw::IClickHandler
  , public bcw::IKeyHandler
{
 public:
  MainMenuScene()           = default;
  ~MainMenuScene() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;
  void onMouse(const ASGE::ClickEvent* mouse_event) override;
  void onKey(const ASGE::KeyEvent* key_event) override;

  void initText();

 private:
  void pressButton(const ASGE::Text* text);

  void updateGamePads();

  std::shared_ptr<GameData> game_data;
  std::unique_ptr<NamePicker> name_picker;
  std::unique_ptr<ASGE::Sprite> background;

  ASGE::Text title_text;
  ASGE::Text play_text;
  ASGE::Text exit_text;

  std::vector<ASGE::Text*> menu_options;
  ASGE::Text* currently_selected_option = nullptr;
};
