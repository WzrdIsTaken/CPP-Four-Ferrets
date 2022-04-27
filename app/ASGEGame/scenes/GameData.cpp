//
// Created by Norbert on 05/04/2022.
//

#include "GameData.hpp"

#include <utility>

using driver = bcw::Driver;

/// ----------------------------------- PUBLIC FUNCTIONS -----------------------------------

void GameData::start()
{
  setDontDestroyOnLoad(true);
  setTag("game_data");
  adventure_font = driver::getRenderer()->loadFont("/data/fonts/Adventure.otf", 100);

  game_pads = std::make_unique<GamePad>(driver::getInputHandler()->getInput());
  if (gp_input.empty())
  {
    for (int i = 0; i < 4; ++i)
    {
      gp_input.emplace_back();
    }
  }
  high_scores = importHighScores();
}

void GameData::update(const ASGE::GameTime& game_time)
{
  updateGamePad(game_time);
}

const ASGE::Font* GameData::getAdventureFont()
{
  return adventure_font;
}

void GameData::resetCharacterData()
{
  characters_selected.clear();
}

void GameData::resetSelectedLevel()
{
  level_selected = GameData::Levels::UNDEFINED;
}

GameData::GamePadInput GameData::getGamePad(uint16_t controller_id)
{
  return gp_input[controller_id];
}

ASGE::Point2D GameData::getScreenSize()
{
  return screen;
}

GameData::Colours GameData::getColours()
{
  return colour_palette;
}

void GameData::setGangName(std::string new_gang_name)
{
  gang_name = std::move(new_gang_name);
}

std::string GameData::getGangName()
{
  return gang_name;
}

void GameData::selectLevel(Levels level)
{
  level_selected = level;
}

std::string GameData::getSelectedLevel()
{
  return levels[static_cast<uint64_t>(level_selected)];
}

void GameData::addCharacter(uint16_t character_id)
{
  characters_selected.push_back(character_id);
}

std::vector<uint16_t>* GameData::getSelectedCharacters()
{
  return &characters_selected;
}

std::vector<GameData::Score>* GameData::getHighScores()
{
  return &high_scores;
}

void GameData::exportScore(uint64_t time_in_sec)
{
  Score to_export;
  to_export.gang_name   = getGangName();
  to_export.time_in_sec = time_in_sec;

  high_scores.emplace_back(to_export);

  std::sort(
    high_scores.begin(),
    high_scores.end(),
    [](const Score& a, const Score& b) { return a.time_in_sec < b.time_in_sec; });

  if (high_scores.size() > 5)
  {
    high_scores.resize(5);
  }

  std::ofstream file;
  file.open("score.txt");
  for (uint16_t i = 0; i < 5; ++i)
  {
    file << high_scores[i].gang_name << std::endl;
    file << high_scores[i].time_in_sec << std::endl;
  }
  file.close();
}

void GameData::playSelectSound()
{
  driver::getAudioHandler()->playAudio("data/audio/menu_select.mp3");
}

void GameData::playButtonSound()
{
  driver::getAudioHandler()->playAudio("data/audio/button_press.mp3", 0.2F);
}

/// ----------------------------------- PRIVATE FUNCTIONS -----------------------------------

void GameData::updateGamePad(const ASGE::GameTime& game_time)
{
  uint16_t id = 0;
  for (auto& gp : gp_input)
  {
    gp.left_stick   = game_pads->leftStickState(id);
    gp.d_pad_up     = game_pads->dPad_UP(id);
    gp.d_pad_down   = game_pads->dPad_DOWN(id);
    gp.d_pad_left   = game_pads->dPad_LEFT(id);
    gp.d_pad_right  = game_pads->dPad_RIGHT(id);
    gp.button_A     = game_pads->buttonA(id);
    gp.button_B     = game_pads->buttonB(id);
    gp.button_X     = game_pads->buttonX(id);
    gp.button_Y     = game_pads->buttonY(id);
    gp.button_back  = game_pads->buttonBack(id);
    gp.button_start = game_pads->buttonStart(id);
    gp.bumper_left  = game_pads->bumperLeft(id);
    gp.bumper_right = game_pads->bumperRight(id);
    ++id;
  }
  game_pads->update(game_time);
}

std::vector<GameData::Score> GameData::importHighScores()
{
  std::vector<Score> to_return;
  if (std::ifstream("score.txt"))
  {
    std::ifstream file("score.txt");
    for (int i = 0; i < 5; ++i)
    {
      std::string time;
      Score score;
      getline(file, score.gang_name);
      getline(file, time);
      score.time_in_sec = static_cast<uint64_t>(std::stoi(time));
      to_return.emplace_back(score);
    }
    file.close();
  }
  else
  {
    to_return.emplace_back(Score{ "Norbert", 90 });
    to_return.emplace_back(Score{ "Ben", 120 });
    to_return.emplace_back(Score{ "Matteo", 150 });
    to_return.emplace_back(Score{ "James", 180 });
    to_return.emplace_back(Score{ "Bob", 210 });
  }
  return to_return;
}
