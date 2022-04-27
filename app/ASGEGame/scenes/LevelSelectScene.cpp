// Created by Ben on 26/02/2022

#include "ASGEGameLib/components/persistence/FileStream.hpp"

#include "CharacterSelectScene.hpp"
#include "LevelSelectScene.hpp"
#include "MainMenuScene.hpp"

void LevelSelectScene::start()
{
  game_data = std::dynamic_pointer_cast<GameData>(driver::getSceneHandler()->findScene("game_"
                                                                                       "data"));

  initText();
  initSprites();
}

void LevelSelectScene::update(const ASGE::GameTime& /*game_time*/)
{
  updateGamePad();
}

void LevelSelectScene::render(const ASGE::GameTime& /*game_time*/)
{
  if (!loading_next_scene)
  {
    bcw::Driver::getRenderer()->render(*background);

    bcw::Driver::getRenderer()->render(title_text);

    bcw::Driver::getRenderer()->render(level1_text);
    bcw::Driver::getRenderer()->render(level2_text);
    bcw::Driver::getRenderer()->render(level3_text);
  }
  else
  {
    bcw::Driver::getRenderer()->render(*loading_screen);

    static int16_t cd = 6;
    if (cd <= 0)
    {
      bcw::Driver::getSceneHandler()->changeScene<CharacterSelectScene>();
    }
    else
    {
      --cd;
    }
  }
}

void LevelSelectScene::onMouse(const ASGE::ClickEvent* mouse_event)
{
  for (const auto& option : level_options)
  {
    if (bcw::Driver::getInputHandler()->checkClick(mouse_event, *option))
    {
      pressButton(option);
    }
  }
}

void LevelSelectScene::initSprites()
{
  loading_screen = bcw::Driver::getRenderer()->createUniqueSprite();
  loading_screen->loadTexture("/data/images/loading.png");

  background = driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    background.get(), "data/images/main_bg.png", bcw::Vector2(0, 0), 1.0F);
  background->setGlobalZOrder(0);
}

void LevelSelectScene::onSelect(int button)
{
  if (button == SELECT_BUTTON)
  {
    pressButton(currently_selected_option);
  }
}

void LevelSelectScene::pressButton(const ASGE::Text* text)
{
  auto it = std::find(level_options.begin(), level_options.end(), text);

  switch (it - level_options.begin())
  {
    case 0: // level_one_button
      loading_next_scene = true;
      game_data->selectLevel(GameData::Levels::LEVEL_1);
      break;
    case 1: // level_two_button
      loading_next_scene = true;
      game_data->selectLevel(GameData::Levels::LEVEL_2);
      break;
    case 2: // level_three_button
      loading_next_scene = true;
      game_data->selectLevel(GameData::Levels::LEVEL_3);
      break;
  }
}

void LevelSelectScene::updateGamePad()
{
  if (game_data->getGamePad(0).d_pad_up || game_data->getGamePad(0).d_pad_down)
  {
    currently_selected_option->setColour(game_data->getColours().base_text);

    auto it = std::find(level_options.begin(), level_options.end(), currently_selected_option);
    int next_button = static_cast<int>(std::distance(level_options.begin(), it)) +
                      (game_data->getGamePad(0).d_pad_down ? 1 : -1);
    int menu_options_size = static_cast<int>(level_options.size());

    if (next_button == -1)
    {
      next_button = menu_options_size - 1;
    }
    else if (next_button == menu_options_size)
    {
      next_button = 0;
    }

    currently_selected_option = level_options[static_cast<unsigned long long>(next_button)];
    currently_selected_option->setColour(game_data->getColours().selected_text);
    game_data->playSelectSound();
  }

  if (game_data->getGamePad(0).button_A)
  {
    auto it = std::find(level_options.begin(), level_options.end(), currently_selected_option);
    game_data->playButtonSound();
    switch (it - level_options.begin())
    {
      case 0: // level_one_button
        loading_next_scene = true;
        game_data->selectLevel(GameData::Levels::LEVEL_1);
        break;
      case 1: // level_two_button
        loading_next_scene = true;
        game_data->selectLevel(GameData::Levels::LEVEL_2);
        break;
      case 2: // level_three_button
        loading_next_scene = true;
        game_data->selectLevel(GameData::Levels::LEVEL_3);
        break;
    }
  }
}

void LevelSelectScene::initText()
{
  title_text.setFont(*game_data->getAdventureFont())
    .setString("Select a level")
    .setColour(normal_colour)
    .setZOrder(1)
    .setPosition({ game_data->getScreenSize().x * 0.5F - title_text.getWidth() * 0.5F, 400.0F });
  level1_text.setFont(*game_data->getAdventureFont())
    .setString("Level 1")
    .setScale(0.75F)
    .setColour(normal_colour)
    .setZOrder(1)
    .setPosition({ game_data->getScreenSize().x * 0.5F - level1_text.getWidth() * 0.5F, 600.0F });
  level2_text.setFont(*game_data->getAdventureFont())
    .setString("Level 2")
    .setScale(0.75F)
    .setColour(normal_colour)
    .setZOrder(1)
    .setPosition({ game_data->getScreenSize().x * 0.5F - level2_text.getWidth() * 0.5F, 670.0F });
  level3_text.setFont(*game_data->getAdventureFont())
    .setString("Level 3")
    .setScale(0.75F)
    .setColour(normal_colour)
    .setZOrder(1)
    .setPosition({ game_data->getScreenSize().x * 0.5F - level3_text.getWidth() * 0.5F, 740.0F });

  level_options             = std::vector<ASGE::Text*>{ &level1_text, &level2_text, &level3_text };
  currently_selected_option = &level1_text;
  currently_selected_option->setColour(selected_colour);
}
