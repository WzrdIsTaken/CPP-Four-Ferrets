// Created by Ben on 18/03/2022

#include "../../../include/ASGEGameLib/containers/Vector2.hpp"

#include "../../../include/ASGEGameLib/handlers/CollisionHandler.hpp"

bcw::CollisionHandler::CollisionHandler(const size_t initial_collidables)
{
  collidables.reserve(initial_collidables);
}

void bcw::CollisionHandler::update(const ASGE::GameTime& /*game_time*/)
{
  if (collidables.empty())
  {
    return;
  }

  for (size_t x = 0; x < collidables.size(); x++)
  {
    for (size_t y = 0; y < x; y++) // y = 1 ??
    {
      // not ideal but i was never meant to be the collision guy ;_;;;
      if (
        collidables[x]->getMarkedForRemoval() || collidables[y]->getMarkedForRemoval() ||
        collidables[x]->getAttachedScene() == nullptr ||
        collidables[y]->getAttachedScene() == nullptr)
      {
        // Basically the problem is that the scene's sprite (what is used for the collision check)
        // is deleted *before* the collider, so checkAABB (which uses a sprite for the check)
        // just dies

        // However... If I rework the collider just to use a bounds instead of a sprite,
        // then the problem could be solved there... idk dude whatever this works I hope
        // I never have to look at this backend system again!!
        continue;
      }

      bool colliding = checkAABB(collidables[x], collidables[y]);

      auto collision_pair = std::make_pair(collidables[x], collidables[y]);
      auto it = std::find(current_collisions.begin(), current_collisions.end(), collision_pair);
      bool already_colliding = it != current_collisions.end();

      // If the objects are not colliding at all then we don't need to do anything
      if (!colliding && !already_colliding)
      {
        continue;
      }

      // If the colliders are already colliding then call onCollisionContinuous
      if ((colliding && already_colliding))
      {
        // This method doesn't exist yet, but it can
      }
      // If the colliders have collided for the first time, call onCollisionEnter
      else if (colliding)
      {
        current_collisions.push_back(collision_pair);

        collidables[x]->on_collision_enter(collidables[y]);
        collidables[y]->on_collision_enter(collidables[x]);
      }
      // If the colliders were colliding but have now stopped, call onCollisionExit
      else
      {
        current_collisions.erase(it);

        collidables[x]->on_collision_exit(collidables[y]);
        collidables[y]->on_collision_exit(collidables[x]);
      }

      /*
      if (checkAABB(collidables[x], collidables[y]))
      {
        collidables[x]->on_collision_enter(collidables[y]);
        collidables[y]->on_collision_enter(collidables[x]);
      }
       */
    }
  }

  handleGarbageCollection();
}

bool bcw::CollisionHandler::addCollider(bcw::Collider* collider)
{
  auto it = std::find(collidables.begin(), collidables.end(), collider);
  if (it == collidables.end())
  {
    collidables.push_back(collider);
    return true;
  }

  return false;
}

bool bcw::CollisionHandler::removeCollider(bcw::Collider* collider)
{
  should_delete_colliders = true;
  collider->setMarkedForRemoval(true);

  return true; // i believe this will never fail :pray:
               // (+ don't want another loop in this operation...)
}

bool bcw::CollisionHandler::clearColliders()
{
  for (auto& collider : collidables)
  {
    collider->setMarkedForRemoval(true);
  }
  return true;
}

bcw::Collider* bcw::CollisionHandler::getLastColliderAdded()
{
  return collidables.back();
}

bool bcw::CollisionHandler::checkAABB(Collider* a, Collider* b)
{
  ASGE::Sprite* a_sprite = a->getAABBCheckSprite();
  ASGE::Sprite* b_sprite = b->getAABBCheckSprite();

  bcw::Vector2 a_pos = { a_sprite->xPos(), a_sprite->yPos() };
  bcw::Vector2 b_pos = { b_sprite->xPos(), b_sprite->yPos() };

  bcw::Vector2 a_size = { a_sprite->width() * a_sprite->scale(),
                          a_sprite->height() * a_sprite->scale() };
  bcw::Vector2 b_size = { b_sprite->width() * b_sprite->scale(),
                          b_sprite->height() * b_sprite->scale() };

  return checkAABB(a_pos.x, a_pos.y, b_pos.x, b_pos.y, a_size.x, a_size.y, b_size.x, b_size.y);
}

bool bcw::CollisionHandler::checkAABB(
  const float a_pos_x, const float a_pos_y, const float b_pos_x, const float b_pos_y,
  const float a_size_x, const float a_size_y, const float b_size_x, const float b_size_y)
{
  return a_pos_x < b_pos_x + b_size_x && a_pos_x + a_size_x > b_pos_x &&
         a_pos_y < b_pos_y + b_size_y && a_pos_y + a_size_y > b_pos_y;
}

void bcw::CollisionHandler::handleGarbageCollection()
{
  if (should_delete_colliders)
  {
    auto it = collidables.begin();

    while (it != collidables.end())
    {
      Collider* collider = (*it);

      if (collider->getMarkedForRemoval())
      {
        it = collidables.erase(it);
        // need to remove?

        removeColliderFromActiveCollisions(collider);
      }
      else
      {
        ++it;
      }
    }

    should_delete_colliders = false;
  }
}
void bcw::CollisionHandler::removeColliderFromActiveCollisions(bcw::Collider* collider)
{
  auto it = current_collisions.begin();
  while (it != current_collisions.end())
  {
    bcw::Collider* collider_1 = (*it).first;
    bcw::Collider* collider_2 = (*it).second;

    if (collider_1 == collider || collider_2 == collider)
    {
      if (collider_1 == collider)
      {
        collider_2->on_collision_exit(collider_1);
      }
      else
      {
        collider_1->on_collision_exit(collider_2);
      }

      it = current_collisions.erase(it);
      // need to remove?
    }
    else
    {
      ++it;
    }
  }
}

std::vector<bcw::Collider*> bcw::CollisionHandler::checkCollisionsInArea(
  const float pos_x, const float pos_y, const float size_x, const float size_y,
  bool call_collision_enter_on_colliders, bcw::Collider* self)
{
  if (call_collision_enter_on_colliders && self == nullptr)
  {
    // Pretty sure you would never want this, but maybe I just can't think of a use case
  }

  std::vector<bcw::Collider*> collided_colliders;

  for (const auto& collider : collidables)
  {
    ASGE::Sprite* collider_sprite = collider->getAABBCheckSprite();
    bcw::Vector2 collider_pos     = { collider_sprite->xPos(), collider_sprite->yPos() }; // This
                                                                                      // function
                                                                                      // took xPos
                                                                                      // for y
                                                                                      // position...
    bcw::Vector2 collider_size = { collider_sprite->width(),
                                   collider_sprite->height() }; // * scale???

    if (checkAABB(
          collider_pos.x,
          collider_pos.y,
          pos_x - size_x * 0.5F + 16.0F,
          pos_y - size_y * 0.5F + 16.0F,
          collider_size.x,
          collider_size.y,
          size_x,
          size_y))
    {
      collided_colliders.push_back(collider);
    }
  }

  if (call_collision_enter_on_colliders)
  {
    for (const auto& collider : collided_colliders)
    {
      collider->on_collision_enter(self);
    }
  }

  return collided_colliders;
}

const std::vector<bcw::Collider*>& bcw::CollisionHandler::getAllCollidables()
{
  return collidables;
}
