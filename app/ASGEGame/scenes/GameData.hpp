//
// Created by Norbert on 05/04/2022.
//

#ifndef ASGEGAME_GAMEDATA_HPP
#define ASGEGAME_GAMEDATA_HPP

#include "ASGEGameLib/Driver.hpp"
#include "ASGEGameLib/components/gamepad/GamePad.hpp"

#include <fstream>
#include <iostream>

class GameData : public bcw::Scene
{
 public:
  struct Colours
  {
    ASGE::Colour base_text     = ASGE::COLOURS::WHITE;
    ASGE::Colour black_text    = ASGE::COLOURS::BLACK;
    ASGE::Colour selected_text = ASGE::COLOURS::ORANGE;
    ASGE::Colour invalid_text  = ASGE::COLOURS::RED;
  };

  struct GamePadInput
  {
    ASGE::Point2D left_stick{ 0.0F, 0.0F };
    bool d_pad_up{ false };
    bool d_pad_down{ false };
    bool d_pad_left{ false };
    bool d_pad_right{ false };
    bool bumper_left{ false };
    bool bumper_right{ false };
    bool button_A{ false };
    bool button_B{ false };
    bool button_X{ false };
    bool button_Y{ false };
    bool button_back{ false };
    bool button_start{ false };
  };

  struct Score
  {
    std::string gang_name = "-";
    uint64_t time_in_sec  = 0;
  };

  enum class Levels
  {
    UNDEFINED = 0,
    LEVEL_1   = 1,
    LEVEL_2   = 2,
    LEVEL_3   = 3
  };

  void start() override;
  void update(const ASGE::GameTime& game_time) override;

  const ASGE::Font* getAdventureFont();
  void resetCharacterData();
  void resetSelectedLevel();

  GamePadInput getGamePad(uint16_t controller_id);

  ASGE::Point2D getScreenSize();

  Colours getColours();

  void setGangName(std::string new_gang_name);
  std::string getGangName();

  void selectLevel(Levels level);
  std::string getSelectedLevel();

  void addCharacter(uint16_t character_id);
  std::vector<uint16_t>* getSelectedCharacters();

  std::vector<Score>* getHighScores();
  void exportScore(uint64_t time_in_sec);

  void playSelectSound();
  void playButtonSound();

 private:
  const ASGE::Font* adventure_font;
  ASGE::Point2D screen{ 1920.0F, 1080.0F };
  std::string gang_name;
  std::unique_ptr<GamePad> game_pads;
  std::vector<GamePadInput> gp_input;

  GameData::Levels level_selected{ Levels::UNDEFINED };
  std::vector<std::string> levels{ "Default.tmx", "Level1.tmx", "Level2.tmx", "Level3.tmx" };
  std::vector<uint16_t> characters_selected;
  Colours colour_palette;

  std::vector<Score> high_scores;

  void updateGamePad(const ASGE::GameTime& game_time);
  std::vector<Score> importHighScores();
};

#endif // ASGEGAME_GAMEDATA_HPP
