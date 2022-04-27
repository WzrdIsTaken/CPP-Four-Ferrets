// Created by Ben on 24/02/2022
// Changes by Norbert... lot of changes actually :D

#include "MainMenuScene.hpp"
#include "LevelSelectScene.hpp"

using driver = bcw::Driver;
#include "../gameobjects/managers/MusicManager.h"
void MainMenuScene::start()
{
  /// Initialising text moved to a function that is
  /// called before the first render cycle happens - Norbert

  background = driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    background.get(), "data/images/main_bg.png", bcw::Vector2(0, 0), 1.0F);
  background->setGlobalZOrder(0);

  // Play music on another thread, so it doesn't stop when loading!
  auto music_manager = bcw::Driver::getSceneHandler()->addScene<MusicManager>();
  music_manager->queueSongs({ "data/audio/insane_music.mp3" });

  std::function<void()> job = [music_manager] { music_manager->playSong(); };
  bcw::Driver::getThreadHandler()->scheduleJob(job); // B) - Ben
}

void MainMenuScene::update(const ASGE::GameTime& /*game_time*/)
{
  if (name_picker != nullptr)
  {
    if (!name_picker->isActive())
    {
      updateGamePads();
    }
  }
}

void MainMenuScene::render(const ASGE::GameTime& game_time)
{
  if (game_data == nullptr)
  {
    game_data = std::dynamic_pointer_cast<GameData>(driver::getSceneHandler()->findScene("game_"
                                                                                         "data"));
    initText();
    name_picker = std::make_unique<NamePicker>(
      game_data->getAdventureFont(), game_data->getColours().black_text);
  }

  driver::getRenderer()->render(*background);
  bcw::Driver::getRenderer()->render(title_text);
  if (name_picker->isActive())
  {
    name_picker->render(game_time);
  }
  else
  {
    bcw::Driver::getRenderer()->render(play_text);
    bcw::Driver::getRenderer()->render(exit_text);
  }
}

void MainMenuScene::onMouse(const ASGE::ClickEvent* mouse_event)
{
  for (const auto& option : menu_options)
  {
    if (bcw::Driver::getInputHandler()->checkClick(mouse_event, *option))
    {
      pressButton(option);
    }
  }
}

void MainMenuScene::onKey(const ASGE::KeyEvent* key_event)
{
  if (key_event->key == ASGE::KEYS::KEY_ENTER && key_event->action == 1 && name_picker->isActive())
  {
    game_data->setGangName(
      !name_picker->getGangName().empty() ? name_picker->getGangName() : "ASGE Enjoyer");

    name_picker->setActiveState(false);
    game_data->playButtonSound();
    bcw::Driver::getSceneHandler()->changeScene<LevelSelectScene>();
  }
  else if (name_picker->isActive())
  {
    name_picker->onKey(key_event);
  }
}

void MainMenuScene::pressButton(const ASGE::Text* text)
{
  auto it = std::find(menu_options.begin(), menu_options.end(), text);

  switch (it - menu_options.begin())
  {
    case 0: // play_text
      name_picker->setActiveState(true);
      break;
    case 1: // exit_text
      bcw::Driver::getDriver()->signalExit();
      break;
  }
}

void MainMenuScene::updateGamePads()
{
  if (game_data->getGamePad(0).d_pad_up || game_data->getGamePad(0).d_pad_down)
  {
    currently_selected_option->setColour(game_data->getColours().base_text);

    auto it = std::find(menu_options.begin(), menu_options.end(), currently_selected_option);
    int next_button = static_cast<int>(std::distance(menu_options.begin(), it)) +
                      (game_data->getGamePad(0).d_pad_down ? 1 : -1);
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
    currently_selected_option->setColour(game_data->getColours().selected_text);

    game_data->playSelectSound();
  }

  if (game_data->getGamePad(0).button_A)
  {
    auto it = std::find(menu_options.begin(), menu_options.end(), currently_selected_option);
    game_data->playButtonSound();
    switch (it - menu_options.begin())
    {
      case 0: // play_text
        name_picker->setActiveState(true);
        break;
      case 1: // exit_text
        bcw::Driver::getDriver()->signalExit();
        break;
    }
  }
}

void MainMenuScene::initText()
{
  title_text.setFont(*game_data->getAdventureFont())
    .setString("Four Ferrets")
    .setColour(game_data->getColours().base_text)
    .setZOrder(1)
    .setPosition({ game_data->getScreenSize().x * 0.5F - title_text.getWidth() * 0.5F, 400.0F });
  play_text.setFont(*game_data->getAdventureFont())
    .setString("Play")
    .setScale(0.75F)
    .setColour(game_data->getColours().base_text)
    .setZOrder(1)
    .setPosition({ game_data->getScreenSize().x * 0.5F - play_text.getWidth() * 0.5F, 600.0F });
  exit_text.setFont(*game_data->getAdventureFont())
    .setString("Exit")
    .setScale(0.75F)
    .setColour(game_data->getColours().base_text)
    .setZOrder(1)
    .setPosition({ game_data->getScreenSize().x * 0.5F - exit_text.getWidth() * 0.5F, 670.0F });

  menu_options              = std::vector<ASGE::Text*>{ &play_text, &exit_text };
  currently_selected_option = &play_text;
  currently_selected_option->setColour(game_data->getColours().selected_text);
}
