//
// Created by Norbert on 11/04/2022.
//

#ifndef ASGEGAME_GAMESCENE_HPP
#define ASGEGAME_GAMESCENE_HPP

#include "/gameobjects/characters/NonPlayerCharacter.hpp"
#include "/gameobjects/characters/PlayerCharacter.hpp"
#include "/gameobjects/characters/playercharacters/Bruiser.hpp"
#include "/gameobjects/characters/playercharacters/Hacker.hpp"
#include "/gameobjects/characters/playercharacters/Marksman.hpp"
#include "/gameobjects/characters/playercharacters/Thief.hpp"

#include "EndOfLevelScene.hpp"
#include "GameData.hpp"

#include "ASGEGameLib/Driver.hpp"
#include "gameobjects/mapstuff/MapManager.hpp"
#include <ASGEGameLib/components/camera/Camera.hpp>
#include <ASGEGameLib/components/gamepad/GamePad.hpp>

class GameScene : public bcw::Scene, public bcw::IKeyHandler
{
 public:
  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;

  void addPlayerData(const std::vector<PlayerCharacter::CharacterClasses>& player_data);

  // Objective logic pt1
  [[nodiscard]] bool getObjectiveCollected() const;
  void setObjectiveCollected(bool _object_collected);
  void onKey(const ASGE::KeyEvent *key_event) override;

  [[nodiscard]] int getAlivePlayers() const;
  void setAlivePlayers(int _alive_players);

  void levelComplete();

 private:
  ASGE::Point2D base_resolution{ 1920.0F, 1080.0F };
  std::shared_ptr<GameData> game_data;
  std::unique_ptr<ASGE::Sprite> hud_grid;
  ASGE::Text timer_text;
  double timer = 0.0F;

  std::unique_ptr<MapManager> map;
  std::vector<std::shared_ptr<PlayerCharacter>> players;
  ASGE::Point2D viewport_size{ base_resolution.x * 0.5F, base_resolution.y * 0.5F };
  std::vector<ASGE::Point2D> screen_quarters{ ASGE::Point2D{ 0.0F, viewport_size.y },
                                              ASGE::Point2D{ viewport_size.x, viewport_size.y },
                                              ASGE::Point2D{ 0.0F, 0.0F },
                                              ASGE::Point2D{ viewport_size.x, 0.0F } };

  ASGE::Camera::CameraView clamp{
    viewport_size.x * 0.5F, viewport_size.y * 0.5F, viewport_size.x * -0.5F, viewport_size.y * -0.5F
  };

  std::vector<PlayerCharacter::CharacterClasses> selected_characters;

  /// FOR TESTING PURPOSES ONLY - will be removed later
  [[maybe_unused]] std::vector<PlayerCharacter::CharacterClasses> test_data{
    PlayerCharacter::CharacterClasses::THIEF,
    PlayerCharacter::CharacterClasses::MARKSMAN,
    PlayerCharacter::CharacterClasses::BRUISER,
    PlayerCharacter::CharacterClasses::HACKER
  };

  void initHUD();
  void updatePlayers(const ASGE::GameTime& game_time, uint16_t player_id);
  std::shared_ptr<PlayerCharacter> characterSelector(PlayerCharacter::CharacterClasses choice);

  void renderViewport(uint16_t cam_index, const ASGE::GameTime& game_time);

  void updateGamePad(uint16_t pad_id);
  void updateClampValue();
  void initPlayers();
  std::vector<PlayerCharacter::CharacterClasses> getPlayerData();
  void renderHud();
  void updateTimer(const ASGE::GameTime& game_time);
  void endOfLevel(bool successful_completion);

  // Pause logic
  void pause();
  bool paused = false;

  // Objective logic pt2
  bool objective_collected = false;
  int alive_players        = 4;

  // Bot logic
  bool init_alarm_manager = true;
  ASGE::Text paused_text;
};

#endif // ASGEGAME_GAMESCENE_HPP
