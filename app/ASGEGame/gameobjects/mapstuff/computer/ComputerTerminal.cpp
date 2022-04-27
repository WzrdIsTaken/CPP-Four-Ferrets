// Created by Ben on 06/04/2022

#include "../../misc/Timer.hpp"

#include "ComputerTerminal.hpp"

void ComputerTerminal::start()
{
  MapObject::start();

  collider = std::make_unique<bcw::Collider>(
    [this](bcw::Collider* collision) { onCollisionEnter(collision); },
    [this](bcw::Collider* collision) { onCollisionExit(collision); },
    sprite.get(),
    this->shared_from_this());
}

void ComputerTerminal::update(const ASGE::GameTime& game_time)
{
  if (hacking)
  {
    hack_progress = hack_progress + static_cast<float>(game_time.deltaInSecs());
    // Logging::INFO(std::to_string(hack_progress));

    if (hack_progress >= hack_time)
    {
      hacking = false;
      hacked  = true;
    }
  }
}

void ComputerTerminal::init(
  const bcw::Vector2& position, const std::vector<std::shared_ptr<IHackable>>& _linked_objects)
{
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), "/data/images/terminal/Terminal.png", position, 1.F);
  linked_objects = _linked_objects;
}

void ComputerTerminal::onInteractionBegin()
{
  if (hacked)
  {
    for (const auto& object : linked_objects)
    {
      object->onHackBegin(0.0F); // 0.0 so its instantly called
    }
  }
}

void ComputerTerminal::onInteractionEnd() {}

void ComputerTerminal::onHackBegin(float _hack_time)
{
  if (!hacked && !hacking) // 10/10
  {
    hacking       = true;
    hack_time     = _hack_time;
    hack_progress = 0.0F;

    std::shared_ptr<Timer> timer = bcw::Driver::getSceneHandler()->addGameObject<Timer>();
    timer->init({ sprite->xPos(), sprite->yPos() }, ASGE::COLOURS::YELLOWGREEN, hack_time);
    bcw::Driver::getAudioHandler()->playAudio("data/audio/game_sfx/hacking.mp3", 0.2F);
  }
}

void ComputerTerminal::onHackEnd() {}

bool ComputerTerminal::getHackedState()
{
  return hacked;
}

void ComputerTerminal::onCollisionEnter(bcw::Collider* /*collision*/) {}

void ComputerTerminal::onCollisionExit(bcw::Collider* /*collision*/) {}
