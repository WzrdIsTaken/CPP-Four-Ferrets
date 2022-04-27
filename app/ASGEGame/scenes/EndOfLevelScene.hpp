//
// Created by Norbert on 11/04/2022.
//

#ifndef ASGEGAME_ENDOFLEVELSCENE_HPP
#define ASGEGAME_ENDOFLEVELSCENE_HPP

#include "../scenes/GameData.hpp"

#include "ASGEGameLib/Driver.hpp"

class EndOfLevelScene : public bcw::Scene
{
 public:
  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;

 private:
  std::shared_ptr<GameData> game_data;

  std::unique_ptr<ASGE::Sprite> background;
  std::unique_ptr<ASGE::Sprite> bye_bye;
  std::unique_ptr<ASGE::Sprite> high_scores_bg;

  ASGE::Text end_of_level_text;
  ASGE::Text replay_level_text;
  ASGE::Text select_level_text;
  ASGE::Text quit_game_text;

  ASGE::Text hs_gang_names_text;
  ASGE::Text hs_time_text;

  std::vector<ASGE::Text*> menu_options;
  ASGE::Text* currently_selected_option;

  ASGE::Colour selected_colour = ASGE::COLOURS::ORANGE;
  ASGE::Colour normal_colour   = ASGE::COLOURS::WHITE;

  bool exit_game = false;

  void initSprites();
  void initText();
  void setProjection();
  void updateGamePads();
  void pressButton(const ASGE::Text* text);
  void moveSelection(bool right);
  void processHighScores();
  std::string formatTime(uint64_t time_in_sec);

  // TODO: Add file managing functions for saving and loading of high scores
};

#endif // ASGEGAME_ENDOFLEVELSCENE_HPP
