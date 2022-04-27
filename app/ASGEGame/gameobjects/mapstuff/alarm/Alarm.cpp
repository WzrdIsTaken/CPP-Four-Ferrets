// Created by Ben on 31/03/2022

#include <ASGEGameLib/HelperMethods.hpp>

#include "../../../helper/ZOrderData.hpp"

#include "Alarm.hpp"

void Alarm::start()
{
  // Init base class
  MapObject::start();

  // Sprite
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "data/images/alarm/alarm_idle_1.png", { 0, 0 }, 1.F);

  light_sprite = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    light_sprite.get(), "data/images/alarm/alarm_light.png", { 0, 0 }, 2.0F);
  light_sprite->setGlobalZOrder(ZOrderData::LIGHTS);
  light_sprite->opacity(0.25F);

  // Animator
  std::map<std::string, std::vector<std::string>> animations = {
    { "idle",
      { "data/images/alarm/alarm_idle_1.png",
        "data/images/alarm/alarm_idle_2.png",
        "data/images/alarm/alarm_idle_3.png",
        "data/images/alarm/alarm_idle_4.png" } },
    { "active",
      { "data/images/alarm/alarm_active_1.png", "data/images/alarm/alarm_active_2.png" } },
    { "disabled", { "data/images/alarm/alarm_disabled.png" } }
  };
  animator = std::make_unique<bcw::Animator>(sprite.get(), animations, 0.12F);
  animator->setAnimation("idle");

  setTag("Alarm");
}

void Alarm::init(const bcw::Vector2& position)
{
  sprite->xPos(position.x);
  sprite->yPos(position.y);

  light_sprite->xPos(position.x - 105.0F); // so its centered
  light_sprite->yPos(position.y);
}

void Alarm::update(const ASGE::GameTime& game_time)
{
  animator->update(game_time);

  light_sprite_rotation += static_cast<float>(SPIN_SPEED * game_time.deltaInSecs());
  if (light_sprite_rotation > 360)
  {
    light_sprite_rotation = 0;
  }
  light_sprite->rotationInRadians(light_sprite_rotation);
}

void Alarm::render(const ASGE::GameTime& game_time)
{
  MapObject::render(game_time);

  if (active && !disabled)
  {
    bcw::Driver::getRenderer()->render(*light_sprite);
  }
}

void Alarm::activate(bool _active)
{
  if (!disabled)
  {
    animator->setAnimation(_active ? "active" : "idle");
  }

  active = _active;
}

void Alarm::disable(bool _disable)
{
  if (_disable && !disabled)
  {
    animator->setAnimation("disabled");
    active = false;
  }
  else
  {
    animator->setAnimation(active ? "active" : "idle");
  }

  disabled = _disable;
}
