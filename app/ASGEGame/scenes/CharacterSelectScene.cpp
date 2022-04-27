//
// Created by Norbert on 17/03/2022.
//

#include "CharacterSelectScene.hpp"

/// ----------------------------- PUBLIC FUNCTIONS -----------------------------

void CharacterSelectScene::start()
{
  game_data = std::dynamic_pointer_cast<GameData>(driver::getSceneHandler()->findScene("game_"
                                                                                       "data"));
  initText();
  initPickers();
  initSprites();
}

void CharacterSelectScene::update(const ASGE::GameTime& /*game_time*/)
{
  for (uint16_t i = 0; i < 4; ++i)
  {
    updateGamePads(i);
  }

  if (checkLockedPickers())
  {
    changeScene();
  }
}

void CharacterSelectScene::render(const ASGE::GameTime& /*game_time*/)
{
  if (!loading_next_scene)
  {
    for (auto& text : players)
    {
      driver::getRenderer()->render(text);
      driver::getRenderer()->render(*background);
      for (auto& picker : character_pickers)
      {
        picker->render();
      }
    }
  }
  else
  {
    bcw::Driver::getRenderer()->render(*loading_screen);
    /// not an elegant solution, this will need to change by release date:
    static int16_t cd = 6;
    if (cd <= 0)
    {
      auto new_scene = driver::getSceneHandler()->changeScene<GameScene>();
      //      new_scene->addPlayerData(selected_characters);
    }
    else
    {
      --cd;
    }
  }
}

/// ----------------------------- PRIVATE FUNCTIONS -----------------------------
void CharacterSelectScene::initSprites()
{
  background = driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    background.get(), "data/images/camera_test.png", bcw::Vector2(0, 0), 1.0F);

  loading_screen = bcw::Driver::getRenderer()->createUniqueSprite();
  loading_screen->loadTexture("/data/images/loading.png");
}

void CharacterSelectScene::initText()
{
  for (uint16_t i = 0; i < 4; ++i)
  {
    auto& text = players.emplace_back();
    text.setFont(*game_data->getAdventureFont())
      .setColour(colour)
      .setString("Player " + std::to_string(i + 1))
      .setPosition(player_text_positions[i])
      .setZOrder(10)
      .setScale(0.45F);
  }
}

void CharacterSelectScene::initPickers()
{
  for (uint16_t i = 0; i < 4; ++i)
  {
    ASGE::Point2D pos{ player_text_positions[i].x + picker_offset.x,
                       player_text_positions[i].y + picker_offset.y };
    character_pickers.emplace_back(
      std::make_unique<CharacterPicker>(pos, game_data->getAdventureFont()));
  }
}

void CharacterSelectScene::updateGamePads(uint16_t pad_id)
{
  if (game_data->getGamePad(pad_id).button_start)
  {
    driver::getDriver()->signalExit();
  }

  if (game_data->getGamePad(pad_id).button_A)
  {
    if (!character_pickers[pad_id]->isLocked())
    {
      game_data->playButtonSound();
      int pick                           = character_pickers[pad_id]->selectCharacter();
      selected_characters[pad_id] = static_cast<PlayerCharacter::CharacterClasses>(pick);
      for (auto& picker : character_pickers)
      {
        if (!picker->isLocked())
        {
          picker->updateList(static_cast<uint16_t>(pick));
        }
      }
      Logging::INFO("Character selected");
    }
  }

  ///  <---- For the release version enable lines 124-140, and remove  lines 143-168 ---->
    if (game_data->getGamePad(pad_id).d_pad_down)
    {
      if (!character_pickers[pad_id]->isLocked())
      {
        game_data->playSelectSound();
        character_pickers[pad_id]->moveSelector(1);
      }
    }

    if (game_data->getGamePad(pad_id).d_pad_up)
    {
      if (!character_pickers[pad_id]->isLocked())
      {
        game_data->playSelectSound();
        character_pickers[pad_id]->moveSelector(-1);
      }
    }

//  if (game_data->getGamePad(pad_id).d_pad_down)
//  {
//    if (!character_pickers[controller_id]->isLocked())
//    {
//      game_data->playSelectSound();
//      character_pickers[controller_id]->moveSelector(1);
//    }
//  }
//
//  if (game_data->getGamePad(pad_id).d_pad_up)
//  {
//    if (!character_pickers[controller_id]->isLocked())
//    {
//      game_data->playSelectSound();
//      character_pickers[controller_id]->moveSelector(-1);
//    }
//  }
//
//  if (game_data->getGamePad(pad_id).bumper_left)
//  {
//    controller_id == 0 ? controller_id = 3 : --controller_id;
//  }
//  if (game_data->getGamePad(0).bumper_right)
//  {
//    controller_id == 3 ? controller_id = 0 : ++controller_id;
//  }
}

bool CharacterSelectScene::checkLockedPickers()
{
  uint16_t counter = 0;
  for (auto& picker : character_pickers)
  {
    if (picker->isLocked())
    {
      ++counter;
    }
  }
  return counter >= character_pickers.size();
}

void CharacterSelectScene::changeScene()
{
  for (const auto& character : selected_characters)
  {
    game_data->addCharacter(static_cast<uint16_t>(character));
  }
  loading_next_scene = true;
}
