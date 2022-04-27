// Created by Ben on 18/03/2022

#pragma once

#include <Engine/Sprite.hpp>

#include <functional>

#include "../../Scene.hpp"

// #include "../../Component.hpp"

namespace bcw
{
  class [[maybe_unused]] Collider // : public bcw::Component = circular dependency hell
  {
   public:
    Collider(
      const std::function<void(Collider*)>& _on_collision_enter,
      const std::function<void(Collider*)>& _on_collision_exit, ASGE::Sprite* sprite,
      const std::shared_ptr<Scene>& scene);
    ~Collider();

    Collider(const Collider&) = delete;
    Collider& operator=(const Collider&) = delete;

    // Return the sprite that the collider is using for AABB collision
    ASGE::Sprite* getAABBCheckSprite();
    // Returns the scene this collider is attached to
    [[maybe_unused]] std::shared_ptr<Scene> getAttachedScene();

    // Collision enter callback
    std::function<void(Collider*)> on_collision_enter;
    // Collision end callback
    std::function<void(Collider*)> on_collision_exit;

    // Sets raycast_target
    [[maybe_unused]] void setRaycastTarget(bool _raycast_target);
    // Returns raycast_target
    [[nodiscard]] bool getRaycastTarget() const;

    // Sets marked_for_removal
    void setMarkedForRemoval(bool _marked_for_removal);
    // Returns marked_for_removal
    [[nodiscard]] bool getMarkedForRemoval() const;

    // Sets bool_wall
    void setBoolWall(bool _bool_wall);
    // Returns bool_wall
    [[nodiscard]] bool getBoolWall() const;

   private:
    ASGE::Sprite* check_sprite;
    std::shared_ptr<Scene> attached_scene;

    // Determines if the collider will be hit by raycasts
    bool raycast_target = true;

    // Determines if the collider will be removed from the collidables vector
    bool marked_for_removal = false;

    // A highly advanced variable only gods can understand
    bool bool_wall = false;
  };
} // namespace bcw
