// Created by Ben on 20/01/2022

#pragma once

#include <Engine/OGLGame.hpp>

#include "handlers/AudioHandler.hpp"
#include "handlers/CollisionHandler.hpp"
#include "handlers/InputHandler.hpp"
#include "handlers/RenderHandler.hpp"
#include "handlers/SceneHandler.hpp"
#include "handlers/ThreadHandler.hpp"

namespace bcw
{
  // The class carrying the rest of the project. Implements a slightly... interesting? design
  // pattern
  class Driver : public ASGE::OGLGame
  {
   public:
    // Defines some base settings that the driver will be initialised with
    struct DriverSettings
    {
      bcw::Vector2Int base_resolution = bcw::Vector2Int(1920, 1080);
      unsigned int max_threads        = std::thread::hardware_concurrency();
    };

    Driver(
      const ASGE::GameSettings& asge_settings, const DriverSettings& driver_settings,
      const std::shared_ptr<bcw::Scene>& initial_scene);
    ~Driver() override = default;

    Driver(const Driver&) = delete;
    Driver& operator=(const Driver&) = delete;

    // In case you need to do anything 'ASGEy'. Much easier than writing a bunch of getters...
    [[maybe_unused]] static Driver* getDriver();
    // Returns the scene_handler
    [[maybe_unused]] static SceneHandler* getSceneHandler();
    // Returns the input_handler
    [[maybe_unused]] static InputHandler* getInputHandler();
    // Returns the thread_handler
    [[maybe_unused]] static ThreadHandler* getThreadHandler();
    // Returns the render_handler [not the same as getRenderer! - that's asge's thing!]
    [[maybe_unused]] static RenderHandler* getRenderHandler();
    // Returns the audio_handler
    [[maybe_unused]] static AudioHandler* getAudioHandler();
    // Returns collision_handler
    [[maybe_unused]] static CollisionHandler* getCollisionHandler();

    // Returns ASGE's renderer
    [[maybe_unused]] static ASGE::Renderer* getRenderer();

   private:
    // Called every frame. Updates all handlers which require it
    void update(const ASGE::GameTime& game_time) override;
    // Called every physic time-step. FixedUpdates all handlers which require it
    void fixedUpdate(const ASGE::GameTime& game_time) override;
    // Called every frame. Renders all handlers which require it
    void render(const ASGE::GameTime& game_time) override;

    // Handlers
    std::unique_ptr<bcw::SceneHandler> scene_handler;
    std::unique_ptr<bcw::InputHandler> input_handler;
    std::unique_ptr<bcw::ThreadHandler> thread_handler;
    std::unique_ptr<bcw::RenderHandler> render_handler;
    std::unique_ptr<bcw::AudioHandler> audio_handler;
    std::unique_ptr<bcw::CollisionHandler> collision_handler;

    // I think this is still a bot idea. Well, at least it will reduce code repetition...
    static Driver* driver_ref; // NOLINT can't be const (i don't think...)
  };
} // namespace bcw
