//
// Created by Norbert on 17/03/2022.
//

#ifndef ASGEGAME_CHARACTERSELECTSCENE_HPP
#define ASGEGAME_CHARACTERSELECTSCENE_HPP

#include "ASGEGameLib/Driver.hpp"
#include "ASGEGameLib/components/player/CharacterPicker.hpp"
#include "GameData.hpp"
#include "GameScene.hpp"
#include <ASGEGameLib/components/gamepad/GamePad.hpp>

#include "../gameobjects/characters/PlayerCharacter.hpp"

using driver = bcw::Driver;

class CharacterSelectScene : public bcw::Scene
{
 public:
  CharacterSelectScene()           = default;
  ~CharacterSelectScene() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;

 private:
  void initSprites();
  void initText();
  void initPickers();
  void updateGamePads(uint16_t pad_id);
  bool checkLockedPickers();
  void changeScene();

  std::shared_ptr<GameData> game_data;

  std::vector<ASGE::Text> players;
  ASGE::Colour colour = ASGE::COLOURS::BLACK;
  std::vector<ASGE::Point2D> player_text_positions{ ASGE::Point2D{ 50.0F, 50.0F },
                                                    ASGE::Point2D{ 990.0F, 50.0F },
                                                    ASGE::Point2D{ 50.0F, 590.0F },
                                                    ASGE::Point2D{ 990.0F, 590.0F } };
  ASGE::Point2D picker_offset{ 100.0F, 100.0F };

  std::unique_ptr<ASGE::Sprite> background;
  std::unique_ptr<ASGE::Sprite> loading_screen;

  bool loading_next_scene = false;

  std::vector<std::unique_ptr<CharacterPicker>> character_pickers;
  [[maybe_unused]]uint16_t controller_id = 0;

  std::vector<PlayerCharacter::CharacterClasses> selected_characters{
    PlayerCharacter::CharacterClasses::THIEF,
    PlayerCharacter::CharacterClasses::THIEF,
    PlayerCharacter::CharacterClasses::THIEF,
    PlayerCharacter::CharacterClasses::THIEF
  };
};

#endif // ASGEGAME_CHARACTERSELECTSCENE_HPP
