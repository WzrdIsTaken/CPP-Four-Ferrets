//
// Created by Norbert on 11/04/2022.
//

#include "EndOfLevelScene.hpp"
#include "CharacterSelectScene.hpp"
#include "LevelSelectScene.hpp"

using driver = bcw::Driver;

/// ------------------------------------ PUBLIC FUNCTIONS ------------------------------------
void EndOfLevelScene::start()
{
  game_data = std::dynamic_pointer_cast<GameData>(driver::getSceneHandler()->findScene("game_"
                                                                                       "data"));

  initSprites();
  initText();
  setProjection();
  processHighScores();
}

void EndOfLevelScene::update(const ASGE::GameTime& /*game_time*/)
{
  if (!exit_game)
  {
    updateGamePads();
  }
}

void EndOfLevelScene::render(const ASGE::GameTime& game_time)
{
  driver::getRenderer()->render(*background);

  if (!exit_game)
  {
    driver::getRenderer()->render(end_of_level_text);
    for (const auto& text : menu_options)
    {
      driver::getRenderer()->render(*text);
    }
    driver::getRenderer()->render(hs_gang_names_text);
    driver::getRenderer()->render(hs_time_text);
    driver::getRenderer()->render(*high_scores_bg);
  }
  else
  {
    /// Upon exiting the game the end credits are shown for 5 seconds
    static float cd = 5.0F;
    if (cd >= 0.0F)
    {
      cd -= static_cast<float>(game_time.deltaInSecs());
      driver::getRenderer()->render(*bye_bye);
    }
    else
    {
      driver::getDriver()->signalExit();
    }
  }
}

/// ------------------------------------ PRIVATE FUNCTIONS ------------------------------------

void EndOfLevelScene::initSprites()
{
  background = driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    background.get(), "data/images/main_bg.png", bcw::Vector2(0, 0), 1.0F);
  background->setGlobalZOrder(0);

  bye_bye = driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    bye_bye.get(),
    "data/images/bye_bye.png",
    bcw::Vector2(game_data->getScreenSize().x * 0.25F, game_data->getScreenSize().y * 0.25F),
    1.0F);
  bye_bye->setGlobalZOrder(2);

  high_scores_bg = driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    high_scores_bg.get(), "data/images/high_scores_bg.png", bcw::Vector2(310.0F, 600.0F), 1.0F);
  high_scores_bg->setGlobalZOrder(2);
}

void EndOfLevelScene::initText()
{
  end_of_level_text.setFont(*game_data->getAdventureFont())
    .setString("Game Over")
    .setColour(normal_colour)
    .setZOrder(1)
    .setPosition(
      { game_data->getScreenSize().x * 0.5F - end_of_level_text.getWidth() * 0.5F, 200.0F });

  replay_level_text.setFont(*game_data->getAdventureFont())
    .setString("Replay")
    .setScale(0.75F)
    .setColour(normal_colour)
    .setZOrder(1)
    .setPosition(
      { game_data->getScreenSize().x * 0.5F - replay_level_text.getWidth() * 0.5F, 350.0F });

  select_level_text.setFont(*game_data->getAdventureFont())
    .setString("Select level")
    .setScale(0.75F)
    .setColour(normal_colour)
    .setZOrder(1)
    .setPosition(
      { game_data->getScreenSize().x * 0.5F - select_level_text.getWidth() * 0.5F, 420.0F });

  quit_game_text.setFont(*game_data->getAdventureFont())
    .setString("Exit")
    .setScale(0.75F)
    .setColour(normal_colour)
    .setZOrder(1)
    .setPosition(
      { game_data->getScreenSize().x * 0.5F - quit_game_text.getWidth() * 0.5F, 490.0F });

  menu_options =
    std::vector<ASGE::Text*>{ &replay_level_text, &select_level_text, &quit_game_text };
  currently_selected_option = &replay_level_text;
  currently_selected_option->setColour(selected_colour);

  hs_gang_names_text.setFont(*game_data->getAdventureFont())
    .setString("")
    .setScale(0.5F)
    .setColour(normal_colour)
    .setZOrder(3)
    .setPosition({ game_data->getScreenSize().x * 0.5F - 600.0F, 720.0F });

  hs_time_text.setFont(*game_data->getAdventureFont())
    .setString("")
    .setScale(0.5F)
    .setColour(normal_colour)
    .setZOrder(3)
    .setPosition({ game_data->getScreenSize().x * 0.5F + 473.0F, 720.0F });
}

void EndOfLevelScene::setProjection()
{
  driver::getRenderer()->setViewport(
    ASGE::Viewport{ static_cast<int32_t>(0.0F),
                    static_cast<int32_t>(0.0F),
                    static_cast<int32_t>(game_data->getScreenSize().x),
                    static_cast<int32_t>(game_data->getScreenSize().y) });
  driver::getRenderer()->setProjectionMatrix(
    0.0F, 0.0F, game_data->getScreenSize().x, game_data->getScreenSize().y);
}

void EndOfLevelScene::updateGamePads()
{
  if (game_data->getGamePad(0).d_pad_up)
  {
    moveSelection(false);
  }
  if (game_data->getGamePad(0).d_pad_down)
  {
    moveSelection(true);
  }
  if (game_data->getGamePad(0).button_A)
  {
    pressButton(currently_selected_option);
  }
}

void EndOfLevelScene::pressButton(const ASGE::Text* text)
{
  auto it = std::find(menu_options.begin(), menu_options.end(), text);
  game_data->playButtonSound();
  switch (it - menu_options.begin())
  {
    case 0: // Replay the current selected level
      bcw::Driver::getSceneHandler()->changeScene<CharacterSelectScene>();
      break;

    case 1: // Select new level
      game_data->resetSelectedLevel();
      bcw::Driver::getSceneHandler()->changeScene<LevelSelectScene>();
      break;

    case 2: // Exit game
      exit_game = true;
      break;
  }
}

void EndOfLevelScene::moveSelection(bool right)
{
  currently_selected_option->setColour(normal_colour);
  game_data->playSelectSound();
  auto it         = std::find(menu_options.begin(), menu_options.end(), currently_selected_option);
  int next_button = static_cast<int>(std::distance(menu_options.begin(), it)) + (right ? 1 : -1);
  int menu_options_size = static_cast<int>(menu_options.size());

  if (next_button == -1)
  {
    next_button = menu_options_size - 1;
  }
  else if (next_button == menu_options_size)
  {
    next_button = 0;
  }

  currently_selected_option = menu_options[static_cast<unsigned long long>(next_button)];
  currently_selected_option->setColour(selected_colour);
}

void EndOfLevelScene::processHighScores()
{
  std::vector<GameData::Score> hs_data = *game_data->getHighScores();

  hs_gang_names_text.setString(
    hs_data[0].gang_name + "\n" + hs_data[1].gang_name + "\n" + hs_data[2].gang_name + "\n" +
    hs_data[3].gang_name + "\n" + hs_data[4].gang_name);

  hs_time_text.setString(
    formatTime(hs_data[0].time_in_sec) + "\n" + formatTime(hs_data[1].time_in_sec) + "\n" +
    formatTime(hs_data[2].time_in_sec) + "\n" + formatTime(hs_data[3].time_in_sec) + "\n" +
    formatTime(hs_data[4].time_in_sec));
}

std::string EndOfLevelScene::formatTime(uint64_t time_in_sec)
{
  auto minutes            = static_cast<uint64_t>(time_in_sec / 60);
  uint64_t seconds        = time_in_sec % 60;
  std::string minutes_str = minutes < 10 ? "0" + std::to_string(minutes) : std::to_string(minutes);
  std::string seconds_str = seconds < 10 ? "0" + std::to_string(seconds) : std::to_string(seconds);
  return minutes_str + ":" + seconds_str;
}
