// Created by Ben on 20/01/2022

#include <Engine/Logger.hpp>

#include "../../include/ASGEGameLib/Driver.hpp"

bcw::Driver* bcw::Driver::driver_ref = nullptr; // NOLINT can't be const (i don't think...)

bcw::Driver::Driver(
  const ASGE::GameSettings& asge_settings, const DriverSettings& driver_settings,
  const std::shared_ptr<bcw::Scene>& initial_scene) :
  OGLGame(asge_settings)
{
  if (driver_ref != nullptr)
  {
    Logging::WARN("Why are you trying to create two drivers?");
    return;
  }

  scene_handler  = std::make_unique<bcw::SceneHandler>();
  input_handler  = std::make_unique<bcw::InputHandler>(this->inputs);
  thread_handler = std::make_unique<bcw::ThreadHandler>(driver_settings.max_threads);
  render_handler =
    std::make_unique<bcw::RenderHandler>(this->renderer, driver_settings.base_resolution);
  audio_handler     = std::make_unique<bcw::AudioHandler>();
  collision_handler = std::make_unique<bcw::CollisionHandler>(100);

  driver_ref = this;
  scene_handler->addScene(initial_scene);
}

bcw::Driver* bcw::Driver::getDriver()
{
  return driver_ref;
}

bcw::SceneHandler* bcw::Driver::getSceneHandler()
{
  return driver_ref->scene_handler.get();
}

bcw::InputHandler* bcw::Driver::getInputHandler()
{
  return driver_ref->input_handler.get();
}

bcw::ThreadHandler* bcw::Driver::getThreadHandler()
{
  return driver_ref->thread_handler.get();
}

bcw::RenderHandler* bcw::Driver::getRenderHandler()
{
  return driver_ref->render_handler.get();
}

bcw::AudioHandler* bcw::Driver::getAudioHandler()
{
  return driver_ref->audio_handler.get();
}

bcw::CollisionHandler* bcw::Driver::getCollisionHandler()
{
  return driver_ref->collision_handler.get();
}

ASGE::Renderer* bcw::Driver::getRenderer()
{
  return driver_ref->renderer.get();
}

void bcw::Driver::update(const ASGE::GameTime& game_time)
{
  input_handler->update(game_time);
  scene_handler->update(game_time);
  audio_handler->update(game_time);
  collision_handler->update(game_time);
}

void bcw::Driver::fixedUpdate(const ASGE::GameTime& game_time)
{
  scene_handler->fixedUpdate(game_time);
}

void bcw::Driver::render(const ASGE::GameTime& game_time)
{
  scene_handler->render(game_time);
}
