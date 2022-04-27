// Created by Ben on 10/03/2022

#include <ASGEGameLib/HelperMethods.hpp>

// For some unknown reason having this in the header / storing
// a reference to it makes everything not nice so just deal with this!
#include "../../scenes/GameScene.hpp"
#include "scenes/GameData.hpp"

#include "PlayerCharacter.hpp"

using driver = bcw::Driver;

void PlayerCharacter::start()
{
  CharacterBase::start();
  camera = std::make_unique<Camera>(
    960.0F,
    540.0F,
    ASGE::Point2D{ 0.0F, 0.0F },
    std::dynamic_pointer_cast<GameData>(driver::getSceneHandler()->findScene("game_data"))
      ->getAdventureFont());
  radius = (getSprite()->width() * getSprite()->scale()) / 2;

  setTag("Player");
}

void PlayerCharacter::update(const ASGE::GameTime& game_time)
{
  CharacterBase::update(game_time);

  camera->update(game_time);
  camera->moveCamera(ASGE::Point2D{ sprite->xPos(), sprite->yPos() });
  camera->getHud()->setAbilityText(calculateAndFormatAbilityCoolDown());
}

void PlayerCharacter::render(const ASGE::GameTime& game_time)
{
  CharacterBase::render(game_time);
}

void PlayerCharacter::onKey(const ASGE::KeyEvent* key_event)
{
  key_map[key_event->key] = key_event->action != ASGE::KEYS::KEY_RELEASED;

  // Up / Down
  if (key_map[MOVE_UP_KEY])
  {
    velocity.y = -1;
  }
  else if (key_map[MOVE_DOWN_KEY])
  {
    velocity.y = 1;
  }
  else
  {
    velocity.y = 0;
  }

  // Left / Right
  if (key_map[MOVE_LEFT_KEY])
  {
    velocity.x = -1;
  }
  else if (key_map[MOVE_RIGHT_KEY])
  {
    velocity.x = 1;
  }
  else
  {
    velocity.x = 0;
  }

  // Ability / Interact
  if (key_map[ABILITY_KEY])
  {
    ability({});
  }
  if (key_map[INTERACT_KEY])
  {
    interact();
  }

  // Scroll Inventory Up / Down
  if (key_map[SCROLL_INV_UP_KEY])
  {
    scrollInventory(CharacterBase::ScrollDirection::UP);
  }
  if (key_map[SCROLL_INV_DOWN_KEY])
  {
    scrollInventory(CharacterBase::ScrollDirection::DOWN);
  }
}

void PlayerCharacter::onMouse(const ASGE::ClickEvent* mouse_event)
{
  if (mouse_event->button == 0 && mouse_event->action == 1)
  {
    useHeldItem({ static_cast<float>(mouse_event->xpos), static_cast<float>(mouse_event->ypos) });
  }
}

void PlayerCharacter::init(const bcw::Vector2& spawn_position)
{
  sprite->xPos(spawn_position.x);
  sprite->yPos(spawn_position.y);
}

void PlayerCharacter::setUpClamp(ASGE::Camera::CameraView clamp_value)
{
  camera->setCameraClamp(clamp_value);
}

void PlayerCharacter::setCameraStartPos(ASGE::Point2D start_pos)
{
  camera->getCamera().lookAt(start_pos);
}

void PlayerCharacter::setPlayerPos(ASGE::Point2D pos)
{
  sprite->xPos(pos.x - sprite->width() * 0.5F * sprite->scale());
  sprite->yPos(pos.y - sprite->height() * 0.5F * sprite->scale());
}

float PlayerCharacter::getSpeed()
{
  return speed;
}

void PlayerCharacter::movePlayer(ASGE::Point2D vector, const ASGE::GameTime& /*game_time*/)
{
  /*
  sprite->xPos(
    static_cast<float>(std::round(sprite->xPos() + vector.x * speed * game_time.deltaInSecs())));
  sprite->yPos(
    static_cast<float>(std::round(sprite->yPos() + vector.y * speed * game_time.deltaInSecs())));*/
  sprite->xPos(vector.x);
  sprite->yPos(vector.y);
}

void PlayerCharacter::rotatePlayer(const float left_stick_x, const float left_stick_y)
{
  sprite->rotationInRadians(atan2(left_stick_y, left_stick_x));
}

Camera* PlayerCharacter::getCamera()
{
  return camera.get();
}

void PlayerCharacter::setID(uint16_t id)
{
  player_id = id;
  camera->setID(id);
}

void PlayerCharacter::onDie()
{
  auto current_level = bcw::Driver::getSceneHandler()->findScene<GameScene>();

  if (current_level == nullptr) // It would be null in a test scene
    return;

  int current_players = current_level->getAlivePlayers();
  current_level->setAlivePlayers(current_players - 1);
}

void PlayerCharacter::onRevive()
{
  auto current_level = bcw::Driver::getSceneHandler()->findScene<GameScene>();

  if (current_level == nullptr) // It would be null in a test scene
    return;

  int current_players = current_level->getAlivePlayers();
  current_level->setAlivePlayers(current_players + 1);
  camera->getHud()->setHealthText(calculateAndFormatHealth());
  camera->getHud()->setHealthText("Health 100%");
}

void PlayerCharacter::heal(int heal)
{
  CharacterBase::heal(heal);
  camera->getHud()->setHealthText(calculateAndFormatHealth());
}

void PlayerCharacter::takeDamage(int damage)
{
  CharacterBase::takeDamage(damage);

  camera->getHud()->setHealthText(calculateAndFormatHealth());
}

std::string PlayerCharacter::calculateAndFormatHealth()
{
  // for some reason making max_health protected causes a memory leak...
  // so we will do this bot fix!

  int bot_max_health = 100;
  switch (character_class)
  {
    case CharacterClasses::THIEF:
      bot_max_health = 100;
      break;
    case CharacterClasses::HACKER:
      bot_max_health = 100;
      break;
    case CharacterClasses::BRUISER:
      bot_max_health = 150;
      break;
    case CharacterClasses::MARKSMAN:
      bot_max_health = 75;
      break;
  }

  float health_as_percent =
    (static_cast<float>(health) / static_cast<float>(bot_max_health) ) * 100.0F;
  std::string formatted_health =
    "Health " + std::to_string(static_cast<int>(health_as_percent)) + "%";

  return formatted_health;
}

std::string PlayerCharacter::calculateAndFormatAbilityCoolDown()
{
  // Ok just realised that we could have just used a getter as well for max_health
  // But oh well lets just have diversity on our solutions :D

  std::string ability_hud_text = "Ability Ready!";

  if (!abilityReady())
  {
    int cool_down    = static_cast<int>((ability_cool_down - getAbilityClock()) / ability_cool_down * 100.0);
//    cool_down = abs(cool_down);
    ability_hud_text = "Cooldown: "+std::to_string(cool_down) + "%";
  }

  return ability_hud_text;
}
