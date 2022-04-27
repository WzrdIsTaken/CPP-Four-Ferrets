// Created by Ben on 20/01/2022

#include "../../../include/ASGEGameLib/handlers/SceneHandler.hpp"
#include "../../../include/ASGEGameLib/Driver.hpp"

void bcw::SceneHandler::update(const ASGE::GameTime& game_time)
{
  if (paused)
    return; // NOLINT one line

  handleSceneAddition();

  for (const auto& scene : scenes)
  {
    if (scene->getActiveState())
    {
      scene->update(game_time);
    }
  }

  for (const auto& object : game_objects)
  {
    if (object->getActiveState())
    {
      object->update(game_time);
    }
  }

  handleSceneRemoval();
}

void bcw::SceneHandler::fixedUpdate(const ASGE::GameTime& game_time)
{
  if (paused)
    return; // NOLINT one line

  for (const auto& scene : scenes)
  {
    if (scene->getActiveState())
    {
      scene->fixedUpdate(game_time);
    }
  }

  for (const auto& object : game_objects)
  {
    if (object->getActiveState())
    {
      object->update(game_time);
    }
  }
}

void bcw::SceneHandler::render(const ASGE::GameTime& game_time)
{
  for (const auto& scene : scenes)
  {
    if (scene->getActiveState())
    {
      scene->render(game_time);
    }
  }
}

void bcw::SceneHandler::renderGameObjects(const ASGE::GameTime& game_time)
{
  for (const auto& object : game_objects)
  {
    if (object->getActiveState())
    {
      object->render(game_time);
    }
  }
}

void bcw::SceneHandler::changeScene(const std::shared_ptr<bcw::Scene>& new_scene)
{
  for (const auto& scene : scenes)
  {
    if (!scene->getDontDestroyOnLoad())
    {
      // scenes_to_delete.push(scene);
      removeScene(scene);
    }
  }
  addScene(new_scene);
  clearGameObjects();
}

std::shared_ptr<bcw::Scene>
bcw::SceneHandler::addScene(const std::shared_ptr<bcw::Scene>& scene, const bool added_in_start)
{
  if (scene == nullptr)
  {
    throw std::invalid_argument("Scenes to be initialized cannot be null!");
  }

  Driver::getInputHandler()->addObservers(scene);
  scene->start();

  // If the scene is added in the start method (or on input for that matter) then it's
  // ok to directly insert it into the vector. It's sometimes convenient to do this
  // because of initialization stuff
  if (added_in_start)
  {
    scenes.emplace_back(scene);
  }
  else
  {
    addition_queue.push(scene);
  }

  return scene;
}

std::shared_ptr<bcw::Scene>
bcw::SceneHandler::addGameObject(const std::shared_ptr<bcw::Scene>& game_object, bool added_in_start)
{
  if (game_object == nullptr)
  {
    throw std::invalid_argument("Game Object to be initialized cannot be null!");
  }

  game_object->start();

  // If the scene is added in the start method (or on input for that matter) then it's
  // ok to directly insert it into the vector. It's sometimes convenient to do this
  // because of initialization stuff
  if (added_in_start)
  {
    game_objects.emplace_back(game_object);
  }
  else
  {
    object_addition_queue.push(game_object);
  }

  return game_object;
}

void bcw::SceneHandler::clearGameObjects()
{
  game_objects.clear();
}

void bcw::SceneHandler::handleSceneAddition()
{
  while (!addition_queue.empty())
  {
    std::shared_ptr<bcw::Scene> scene = addition_queue.front();
    addition_queue.pop();

    scenes.emplace_back(scene);
  }

  while (!object_addition_queue.empty())
  {
    std::shared_ptr<bcw::Scene> scene = object_addition_queue.front();
    object_addition_queue.pop();

    game_objects.emplace_back(scene);
  }
}

bool bcw::SceneHandler::removeScene(const std::shared_ptr<bcw::Scene>& scene)
{
  auto it = std::find(scenes.begin(), scenes.end(), scene);
  if (it != scenes.end())
  {
    scene->setActiveState(false);
    // Driver::getInputHandler()->removeObservers(scene);
    // todo fix this :p but doesn't seem to cause any problem
    scene->end();

    should_delete_scenes = true;
    scene->setMarkedForDeletion(true);
    return true;
  }

  return false;
}

bool bcw::SceneHandler::removeGameObject(const std::shared_ptr<bcw::Scene>& game_object)
{
  auto it = std::find(game_objects.begin(), game_objects.end(), game_object);
  if (it != game_objects.end())
  {
    game_object->setActiveState(false);
    game_object->end();

    should_delete_objects = true;
    game_object->setMarkedForDeletion(true);
    return true;
  }

  return false;
}

void bcw::SceneHandler::handleSceneRemoval()
{
  if (should_delete_scenes)
  {
    auto it = scenes.begin();

    while (it != scenes.end())
    {
      if ((*it)->getMarkedForDeletion())
      {
        it = scenes.erase(it);
        // need to remove?
      }
      else
      {
        ++it;
      }
    }

    should_delete_scenes = false;
  }

  if (should_delete_objects)
  {
    auto it = game_objects.begin();

    while (it != game_objects.end())
    {
      if ((*it)->getMarkedForDeletion())
      {
        it = game_objects.erase(it);
        // need to remove?
      }
      else
      {
        ++it;
      }
    }
    should_delete_objects = false;
  }
}

bool bcw::SceneHandler::getPausedState() const
{
  return paused;
}

void bcw::SceneHandler::setPausedState(const bool _paused) // NOLINT _paused
{
  paused = _paused;
}

std::shared_ptr<bcw::Scene> bcw::SceneHandler::findScene(const std::string& tag)
{
  for (const auto& scene : scenes)
  {
    if (scene->getTag() == tag)
    {
      return scene;
    }
  }

  return nullptr;
}

std::vector<std::shared_ptr<bcw::Scene>> bcw::SceneHandler::findScenes(const std::string& tag)
{
  std::vector<std::shared_ptr<bcw::Scene>> found_scenes;

  for (const auto& scene : scenes)
  {
    if (scene->getTag() == tag)
    {
      found_scenes.push_back(scene);
    }
  }

  return found_scenes;
}

std::vector<std::shared_ptr<bcw::Scene>> bcw::SceneHandler::findGameObjects(const std::string& tag)
{
  std::vector<std::shared_ptr<bcw::Scene>> found_objects;

  for (const auto& object : scenes)
  {
    if (object->getTag() == tag)
    {
      found_objects.push_back(object);
    }
  }

  return found_objects;
}
