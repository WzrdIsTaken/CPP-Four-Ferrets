//
// Created by Norbert on 11/04/2022.
//

#include "gameobjects/mapstuff/alarm/AlarmManager.hpp"

#include "GameScene.hpp"

using driver = bcw::Driver;

void GameScene::start()
{
  game_data = std::dynamic_pointer_cast<GameData>(driver::getSceneHandler()->findScene("game_"
                                                                                       "data"));

  driver::getRenderer()->setBaseResolution(
    int(base_resolution.x), int(base_resolution.y), ASGE::Resolution::Policy::MAINTAIN);

  initHUD();

  map = std::make_unique<MapManager>(1, game_data->getSelectedLevel());
  updateClampValue();

  addPlayerData(getPlayerData());
}

void GameScene::update(const ASGE::GameTime& game_time)
{
  if (init_alarm_manager)
  {
    bcw::Driver::getSceneHandler()->findScene<AlarmManager>()->init();
    init_alarm_manager = false;
  }
  /// Updating the controllers data
  for (uint16_t i = 0; i < 4; ++i)
  {
    updateGamePad(i);
  }

  /// Updating the players state
  for (uint16_t i = 0; i < 4; ++i)
  {
    if (!players[i]->getIsDead())
    {
      updatePlayers(game_time, i);
    }
  }
}

void GameScene::render(const ASGE::GameTime& game_time)
{
  /// First step is to update the timer and draw the HUD
  if (!paused)
  {
    updateTimer(game_time);
  }

  renderHud();

  //  const auto& map_objects = bcw::Driver::getSceneHandler()->findScenes<MapObject>();
  //  const auto& non_player_characters =
  //    bcw::Driver::getSceneHandler()->findScenes<NonPlayerCharacter>();

  /// Then we iterate through all the players and render their viewports
  for (uint16_t i = 0; i < 4; ++i)
  {
    renderViewport(i, game_time);
  }
}

void GameScene::addPlayerData(const std::vector<PlayerCharacter::CharacterClasses>& player_data)
{
  for (const auto& data : player_data)
  {
    selected_characters.emplace_back(data);
  }

  initPlayers();
}

void GameScene::initHUD()
{
  hud_grid = driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    hud_grid.get(), "data/images/game_hud.png", bcw::Vector2(0, 0), 1.0F);
  hud_grid->setGlobalZOrder(20);

  timer_text.setFont(*game_data->getAdventureFont())
    .setColour(ASGE::COLOURS::BLACK)
    .setString("99:99 ")
    .setScale(0.35F)
    .setZOrder(21);
  timer_text.setPosition({ game_data->getScreenSize().x * 0.5F - timer_text.getWidth() * 0.5F,
                           game_data->getScreenSize().y * 0.5F + 15.0F });

  paused_text.setFont(*game_data->getAdventureFont())
    .setColour(ASGE::COLOURS::RED)
    .setString("PAUSED")
    .setScale(2.0F)
    .setZOrder(22);
  paused_text.setPosition({ game_data->getScreenSize().x * 0.5F - paused_text.getWidth() * 0.5F,
                           game_data->getScreenSize().y * 0.5F + paused_text.getHeight() * 0.5F});
}

void GameScene::updatePlayers(const ASGE::GameTime& game_time, uint16_t player_id)
{
  ASGE::Point2D move_vector = map->checkCollision(
    *players[player_id]->getSprite(),
    players[player_id]->radius,
    players[player_id]->getSpeed(),
    game_data->getGamePad(player_id).left_stick,
    game_time);

  if (
    std::abs(game_data->getGamePad(player_id).left_stick.x) > 0.0F ||
    std::abs(game_data->getGamePad(player_id).left_stick.y) > 0.0F)
  {
    players[player_id]->rotatePlayer(
      game_data->getGamePad(player_id).left_stick.x, game_data->getGamePad(player_id).left_stick.y);
  }
  players[player_id]->movePlayer({ move_vector }, game_time);
}

void GameScene::renderViewport(uint16_t cam_index, const ASGE::GameTime& game_time)
{
  bcw::Driver::getRenderer()->setViewport(
    ASGE::Viewport{ static_cast<int32_t>(screen_quarters[cam_index].x),
                    static_cast<int32_t>(screen_quarters[cam_index].y),
                    static_cast<int32_t>(viewport_size.x),
                    static_cast<int32_t>(viewport_size.y) });

  players[cam_index]->update(game_time);
  map->renderMap();
  driver::getSceneHandler()->renderGameObjects(game_time);
  players[cam_index]->getCamera()->render();
}

void GameScene::updateGamePad(uint16_t pad_id)
{
  if (game_data->getGamePad(pad_id).button_start)
  {
    driver::getDriver()->signalExit();
  }
  // TODO: remove this once the logic has been added to trigger this scene change,
  //  this is for testing purposes only
  if (game_data->getGamePad(pad_id).button_back)
  {
    levelComplete();
  }

  if (game_data->getGamePad(pad_id).button_A)
  {
    players[pad_id]->ability({ game_data->getGamePad(pad_id).left_stick.x,
                               game_data->getGamePad(pad_id).left_stick.y });
  }

  if (game_data->getGamePad(pad_id).button_B)
  {
    players[pad_id]->interact();
  }

  if (game_data->getGamePad(pad_id).button_X)
  {
    /*
    players[pad_id]->useHeldItem(
      { players[pad_id]->getSprite()->xPos() + game_data->getGamePad(pad_id).left_stick.x,
        players[pad_id]->getSprite()->yPos() + game_data->getGamePad(pad_id).left_stick.y });*/
    players[pad_id]->useHeldItem(
      { game_data->getGamePad(pad_id).left_stick.x,
         game_data->getGamePad(pad_id).left_stick.y });
  }

  if (game_data->getGamePad(pad_id).button_Y)
  {
    players[pad_id]->dropHeldItem();
  }
}

void GameScene::updateClampValue()
{
  clamp.max_x += map->getMapSize().x;
  clamp.max_y += map->getMapSize().y;
}

std::shared_ptr<PlayerCharacter>
GameScene::characterSelector(PlayerCharacter::CharacterClasses choice)
{
  switch (choice)
  {
    case PlayerCharacter::CharacterClasses::THIEF:
    {
      return driver::getSceneHandler()->addGameObject<Thief>();
    }
    case PlayerCharacter::CharacterClasses::HACKER:
    {
      return driver::getSceneHandler()->addGameObject<Hacker>();
    }
    case PlayerCharacter::CharacterClasses::BRUISER:
    {
      return driver::getSceneHandler()->addGameObject<Bruiser>();
    }
    case PlayerCharacter::CharacterClasses::MARKSMAN:
    {
      return driver::getSceneHandler()->addGameObject<Marksman>();
    }
  }
}

void GameScene::initPlayers()
{
  for (uint16_t i = 0; i < 4; ++i)
  {
    std::shared_ptr<PlayerCharacter> player = characterSelector(selected_characters[i]);
    player->setID(i);
    player->setUpClamp(clamp);
    player->setPlayerPos(map->getPlayerStartPos(i));
    player->getSprite()->setGlobalZOrder(3);
    players.emplace_back(player);
  }
}

std::vector<PlayerCharacter::CharacterClasses> GameScene::getPlayerData()
{
  std::vector<PlayerCharacter::CharacterClasses> to_return;
  for (auto& character : *game_data->getSelectedCharacters())
  {
    to_return.emplace_back(static_cast<PlayerCharacter::CharacterClasses>(character));
  }
  return to_return;
}

void GameScene::renderHud()
{
  driver::getRenderer()->setViewport(
    ASGE::Viewport{ static_cast<int32_t>(0.0F),
                    static_cast<int32_t>(0.0F),
                    static_cast<int32_t>(game_data->getScreenSize().x),
                    static_cast<int32_t>(game_data->getScreenSize().y) });
  driver::getRenderer()->setProjectionMatrix(
    0.0F, 0.0F, game_data->getScreenSize().x, game_data->getScreenSize().y);
  driver::getRenderer()->render(*hud_grid);
  driver::getRenderer()->render(timer_text);

  if (paused)
  {
    driver::getRenderer()->render(paused_text);
  }
}

void GameScene::updateTimer(const ASGE::GameTime& game_time)
{
  timer += game_time.deltaInSecs();
  int minutes             = int(timer / 60);
  int seconds             = int(timer) % 60;
  std::string minutes_str = minutes < 10 ? "0" + std::to_string(minutes) : std::to_string(minutes);
  std::string seconds_str = seconds < 10 ? "0" + std::to_string(seconds) : std::to_string(seconds);
  timer_text.setString(minutes_str + ":" + seconds_str);
  timer_text.setPositionX(game_data->getScreenSize().x * 0.5F - timer_text.getWidth() * 0.5F);
}

void GameScene::endOfLevel(const bool successful_completion)
{
  if (successful_completion)
  {
    game_data->exportScore(static_cast<uint64_t>(timer));
  }
  bcw::Driver::getSceneHandler()->changeScene<EndOfLevelScene>();
}

void GameScene::pause()
{
  // Todo: Call this function when the pause key is pressed (not sure how you want to handle this)
  // I think its easier to always have the text being rendered and just toggle the alpha value

  // If the resume button is pressed, call pause again
  // If the return to menu button is pressed, change the scene to level select scene and reset
  // gamedata

  // float alpha = paused ? 1.0F : 0.0F;

  // paused_text.setOpacity(alpha);
  // resume_text.setOpacity(alpha);
  // return_to_menu_text.setOpacity(alpha);

  paused = !paused;
  bcw::Driver::getSceneHandler()->setPausedState(paused);
}

bool GameScene::getObjectiveCollected() const
{
  return objective_collected;
}

void GameScene::setObjectiveCollected(const bool _objective_collected) // NOLINT
{
  objective_collected = _objective_collected;
}

int GameScene::getAlivePlayers() const
{
  return alive_players;
}

void GameScene::setAlivePlayers(const int _alive_players) // NOLINT
{
  alive_players = _alive_players;

  // assert(alive_players == 5 && "Alive players is 5!  Something has gone very wrong...");
  // assert(alive_players == -1 && "Alive players is -1! Something has gone very wrong...");

  if (alive_players == 0)
  {
    endOfLevel(false);
  }
}

void GameScene::levelComplete()
{
  game_data->resetCharacterData();
  driver::getCollisionHandler()->clearColliders();
  endOfLevel(true);

  // (logic calculating if this is valid takes place in EndPoint)
}

void GameScene::onKey(const ASGE::KeyEvent* key_event)
{
  if(key_event->key==ASGE::KEYS::KEY_P && key_event->action == ASGE::KEYS::KEY_PRESSED)
  {
    pause();
  }
}
