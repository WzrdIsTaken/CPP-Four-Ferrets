// Created by Ben on 20/01/2022

#pragma once

#include <queue>
#include <vector>

#include "../Scene.hpp"

// Handles the creation and deletion of scenes (/gameobjects)

namespace bcw
{
  class SceneHandler
  {
   public:
    SceneHandler()  = default;
    ~SceneHandler() = default;

    SceneHandler(const SceneHandler&) = delete;
    SceneHandler& operator=(const SceneHandler&) = delete;

    // Calls update on all active scenes (if paused is false)
    void update(const ASGE::GameTime& game_time);
    // Calls fixedUpdates on all active scenes (if paused is false)
    void fixedUpdate(const ASGE::GameTime& game_time);
    // Calls render on all active scenes (regardless is paused is true or false)
    void render(const ASGE::GameTime& game_time);

    void renderGameObjects(const ASGE::GameTime& game_time);

    // Deletes all current scenes not marked as don't destroy on load and loads the new_scene
    void changeScene(const std::shared_ptr<bcw::Scene>& new_scene);

    template<typename T>
    [[maybe_unused]] std::shared_ptr<T> changeScene()
    {
      std::shared_ptr<T> scene = std::make_shared<T>();
      changeScene(scene);
      return scene;
    }

    // Adds a new scene to the scenes vector
    std::shared_ptr<bcw::Scene>
    addScene(const std::shared_ptr<bcw::Scene>& scene, bool added_in_start = false);

    template<typename T>
    [[maybe_unused]] std::shared_ptr<T> addScene(bool added_in_start = false)
    {
      std::shared_ptr<T> scene = std::make_shared<T>();
      addScene(scene, added_in_start);
      return scene;
    }

    std::shared_ptr<bcw::Scene>
    addGameObject(const std::shared_ptr<bcw::Scene>& game_object, bool added_in_start = false);

    template<typename T>
    [[maybe_unused]] std::shared_ptr<T> addGameObject(bool added_in_start = false)
    {
      std::shared_ptr<T> game_object = std::make_shared<T>();
      addGameObject(game_object, added_in_start);
      return game_object;
    }

    // Tries to delete the scene from the scenes vector, returning true/false depending upon if it
    // was a success
    [[maybe_unused]] bool removeScene(const std::shared_ptr<bcw::Scene>& scene);

    [[maybe_unused]] bool removeGameObject(const std::shared_ptr<bcw::Scene>& scene);

    // Finds a scene based off its tag
    [[maybe_unused]] std::shared_ptr<bcw::Scene> findScene(const std::string& tag);

    // Finds all the scenes with a tag
    [[maybe_unused]] std::vector<std::shared_ptr<bcw::Scene>> findScenes(const std::string& tag);
    [[maybe_unused]] std::vector<std::shared_ptr<bcw::Scene>>
    findGameObjects(const std::string& tag);

    // Finds a scene based off its type
    template<typename T>
    [[maybe_unused]] std::shared_ptr<T> findScene()
    {
      for (const auto& scene : scenes)
      {
        auto scene_of_type = std::dynamic_pointer_cast<T>(scene);
        if (scene_of_type != nullptr)
        {
          return scene_of_type;
        }
      }

      return nullptr;
    }

    // Finds a game object based off its type
    template<typename T>
    [[maybe_unused]] std::shared_ptr<T> findGameObject()
    {
      for (const auto& object : game_objects)
      {
        auto object_of_type = std::dynamic_pointer_cast<T>(object);
        if (object_of_type != nullptr)
        {
          return object_of_type;
        }
      }

      return nullptr;
    }

    // Finds all scenes of a type
    template<typename T>
    [[maybe_unused]] std::vector<std::shared_ptr<T>> findScenes()
    {
      std::vector<std::shared_ptr<T>> found_scenes;

      for (const auto& scene : scenes)
      {
        auto scene_of_type = std::dynamic_pointer_cast<T>(scene);
        if (scene_of_type != nullptr)
        {
          found_scenes.push_back(scene_of_type);
        }
      }

      return found_scenes;
    }
    // Finds all game objects of a type - Norbert
    template<typename T>
    [[maybe_unused]] std::vector<std::shared_ptr<T>> findGameObjects()
    {
      std::vector<std::shared_ptr<T>> found_objects;

      for (const auto& object : game_objects)
      {
        auto scene_of_type = std::dynamic_pointer_cast<T>(object);
        if (scene_of_type != nullptr)
        {
          found_objects.push_back(scene_of_type);
        }
      }

      return found_objects;
    }

    // Returns paused
    [[maybe_unused]] [[nodiscard]] bool getPausedState() const;
    // Sets paused
    [[maybe_unused]] void setPausedState(bool _paused);

    void clearGameObjects();

   private:
    // Praise Norbert :pray: Praise James :pray:
    void handleSceneAddition();
    // Deletes all marked scenes at the end of the frame
    void handleSceneRemoval();

    // Scenes
    std::queue<std::shared_ptr<bcw::Scene>> addition_queue;
    std::vector<std::shared_ptr<bcw::Scene>> scenes;
    std::queue<std::shared_ptr<bcw::Scene>> object_addition_queue;
    std::vector<std::shared_ptr<bcw::Scene>> game_objects;

    // Controls whether scenes are updated
    bool paused = false;

    // Are their scenes waiting to be deleted?
    bool should_delete_scenes  = false;
    bool should_delete_objects = false;
  };
} // namespace bcw
