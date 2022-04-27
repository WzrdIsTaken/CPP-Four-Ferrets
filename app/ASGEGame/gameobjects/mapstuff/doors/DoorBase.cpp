// Created by Ben on 16/03/2022

#include "../../misc/Timer.hpp"

#include "DoorBase.hpp"

void DoorBase::start()
{
  MapObject::start();

  collider = std::make_unique<bcw::Collider>(
    [this](bcw::Collider* collision) { onCollisionEnter(collision); },
    [this](bcw::Collider* collision) { onCollisionExit(collision); },
    sprite.get(),
    this->shared_from_this());
}

void DoorBase::init(
  const std::string& sprite_file_path, const bcw::Vector2& position,
  const bcw::Vector2& open_position, const bool start_open, const bool start_locked)
{
  // Sprite
  bcw::Driver::getRenderHandler()->setupSprite(sprite.get(), sprite_file_path, position, 1.F);

  // Open / closed positions
  start_location = position;
  end_location   = open_position;

  // Initial values
  open   = start_open;
  locked = start_locked;
}

void DoorBase::update(const ASGE::GameTime& game_time)
{
  if (moving)
  {
    bcw::Vector2 move_direction;
    move_direction.x = target_location.x - sprite->xPos();
    move_direction.y = target_location.y - sprite->yPos();
    move_direction.normalise();

    auto delta_as_float = static_cast<float>(game_time.deltaInSecs());
    sprite->xPos(sprite->xPos() + move_direction.x * speed * delta_as_float);
    sprite->yPos(sprite->yPos() + move_direction.y * speed * delta_as_float);

    if (target_location.distance({ sprite->xPos(), sprite->yPos() }) < 1)
    {
      moving = false;
    }
  }

  if (unlocking)
  {
    unlock_progress = unlock_progress + static_cast<float>(game_time.deltaInSecs());
    // Logging::INFO(std::to_string(unlock_progress));

    if (unlock_progress >= unlock_time)
    {
      unlocking = false;
      locked    = false; // !locked;
    }
  }
}

void DoorBase::toggleDoor()
{
  if (!locked)
  {
    open            = !open;
    moving          = true;
    target_location = open ? end_location : start_location;
    bcw::Driver::getAudioHandler()->playAudio("data/audio/game_sfx/open_doors.mp3", 0.4F);
  }
}

void DoorBase::toggleUnlock(const float _unlock_time) // NOLINT
{
  if (moving || !locked || unlocking) // i have no idea what im doing
  {
    return;
  }

  unlocking       = true;
  unlock_time     = _unlock_time;
  unlock_progress = 0.0F;

  std::shared_ptr<Timer> timer = bcw::Driver::getSceneHandler()->addGameObject<Timer>();
  timer->init({ sprite->xPos(), sprite->yPos() }, ASGE::COLOURS::GREEN, unlock_time);
}

bool DoorBase::checkLockedState() const
{
  return locked;
}

void DoorBase::onCollisionEnter(bcw::Collider* collision) {}

void DoorBase::onCollisionExit(bcw::Collider* collision) {}
