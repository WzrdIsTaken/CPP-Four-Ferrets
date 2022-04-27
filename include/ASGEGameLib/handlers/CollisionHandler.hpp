// Created by Ben on 18/03/2022

#pragma once

#include <Engine/GameTime.hpp>

#include "../components/collision/Collider.hpp"

// This collision handling is VERY BAD
// James please implement your system so we never have to use this...

// Having bcw::Vector2 in the header causes circular dependency I think and I cba to untangle
// it rn so will just use floats

// why is the raycasting not in the CollisionHandler?
// Mainly because its unknown atm how much James will add to the system, and having it in
// its own component means that it's nicely encapsulated while any potential tests / overhauls
// take place. Collision is a key system in the game, so it would be best if it didn't break
// unexpectedly...
// But yeah it makes a lot more sense for it to be!

namespace bcw
{
  class CollisionHandler
  {
   public:
    explicit CollisionHandler(size_t initial_collidables);
    ~CollisionHandler() = default;

    CollisionHandler(const CollisionHandler&) = delete;
    CollisionHandler& operator=(const CollisionHandler&) = delete;

    // Checks collisions
    void update(const ASGE::GameTime& game_time);

    // Checks collisions in an area
    [[maybe_unused]] [[nodiscard]] std::vector<Collider*> checkCollisionsInArea(
      float pos_x, float pos_y, float size_x, float size_y,
      bool call_collision_enter_on_colliders = false, bcw::Collider* self = nullptr);

    // Adds a collider to the collidables vector
    // NOTE: This is done for you when you create a collider component!
    [[maybe_unused]] bool addCollider(Collider* collider);
    // Removes a collider from the collidables vector
    // NOTE: This is done for you when you destroy a collider component!
    [[maybe_unused]] bool removeCollider(Collider* collider);

    // Norbert's quick patch...
    bool clearColliders();
    Collider* getLastColliderAdded();

    // Won't be needed
    [[nodiscard]] std::vector<Collider*> const& getAllCollidables();

   private:
    bool checkAABB(Collider* a, Collider* b);
    bool checkAABB(
      float a_pos_x, float a_pos_y, float b_pos_x, float b_pos_y, float a_size_x, float a_size_y,
      float b_size_x, float b_size_y);

    // --- This is well expensive cleanup
    void handleGarbageCollection();
    void removeColliderFromActiveCollisions(Collider* collider);
    // ): ---

    std::vector<Collider*> collidables;
    std::vector<std::pair<Collider*, Collider*>> current_collisions;

    bool should_delete_colliders = false;
  };
} // namespace bcw
