// Created by Ben on 24/03/2022

#include "../../characters/CharacterBase.hpp"

#include "PickupableCollectable.hpp"

void PickupableCollectable::start()
{
  CollectableBase::start();

  icon_sprite = bcw::Driver::getRenderer()->createUniqueSprite();
}

void PickupableCollectable::render(const ASGE::GameTime& game_time)
{
  if (!is_picked_up)
  {
    MapObject::render(game_time);
  }
}

void PickupableCollectable::forceMove(
  const ASGE::GameTime& /*game_time*/, const bcw::Vector2& position)
{
  sprite->xPos(position.x + picked_up_offset.x);
  sprite->yPos(position.y + picked_up_offset.y);
}

void PickupableCollectable::forceRotate(
  const ASGE::GameTime& /*game_time*/, const bcw::Vector2& rotation)
{
  sprite->rotationInRadians(atan2(rotation.x, -rotation.y));
}

void PickupableCollectable::forceRender(const ASGE::GameTime& /*game_time*/)
{
  bcw::Driver::getRenderer()->render(*sprite);
}

void PickupableCollectable::onCollisionEnter(bcw::Collider* collision)
{
  if (collision->getAttachedScene()->getTag() == "Player" && !is_picked_up)
  {
    std::shared_ptr<CharacterBase> player =
      std::dynamic_pointer_cast<CharacterBase>(collision->getAttachedScene());

    // Make sure the player can't have multiple of the same item
    std::vector<std::shared_ptr<PickupableCollectable>> const& player_inv = player->getInventory();
    for (const auto& item : player_inv)
    {
      if (item->getTag() == getTag())
      {
        return;
      }
    }

    is_picked_up = true;
    player->pickUpItem(std::dynamic_pointer_cast<PickupableCollectable>(this->shared_from_this()));
    colliderInCollidables->setMarkedForRemoval(true);
    colliderInCollidables = nullptr;
  }
}

void PickupableCollectable::onCollisionExit(bcw::Collider* /*collision*/) {}

void PickupableCollectable::removeCollectableFromPlayersInventory()
{
  if (attached_to_scene == nullptr)
  {
    // ngl I have no idea why this is needed but *sometimes* attached_to_scene was null
    // so this fixes it
    return;
  }

  std::dynamic_pointer_cast<CharacterBase>(attached_to_scene)
    ->removeItem(std::dynamic_pointer_cast<PickupableCollectable>(this->shared_from_this()), true);
}

void PickupableCollectable::setAttachedToScene(const std::shared_ptr<bcw::Scene>& _attached_to_scene)
{
  attached_to_scene = _attached_to_scene;
}

ASGE::Sprite const* PickupableCollectable::getIconSprite() const
{
  return icon_sprite.get();
}

bool PickupableCollectable::getIsPickedUp() const
{
  return is_picked_up;
}

void PickupableCollectable::setIsPickedUp(const bool _is_picked_up) // NOLINT
{
  is_picked_up = _is_picked_up;
}
