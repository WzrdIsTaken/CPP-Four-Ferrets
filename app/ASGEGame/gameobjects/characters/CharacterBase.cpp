// Created by Ben on 10/03/2022

//#include <ASGEGameLib/HelperMethods.hpp>
#include "../../helper/ZOrderData.hpp"

#include "../mapstuff/IInteractable.hpp"
#include "CharacterBase.hpp"

void CharacterBase::start()
{
  sprite = bcw::Driver::getRenderer()->createUniqueSprite();
  sprite->setGlobalZOrder(ZOrderData::CHARACTERS);

  collider = std::make_unique<bcw::Collider>(
    [this](bcw::Collider* collision) { onCollisionEnter(collision); },
    [this](bcw::Collider* collision) { onCollisionExit(collision); },
    sprite.get(),
    this->shared_from_this());

  max_health    = health;
  ability_clock = 1000.0;
}

void CharacterBase::update(const ASGE::GameTime& game_time)
{
  // Temporarily turned off to prevent character rotation to be jerky
  // However, it is still ticked once in takeDamage and handleRevive because the animator
  // handles the sprite transition alive -> dead and back
  // animator->update(game_time);

  if (!handleKnockOut(game_time.deltaInSecs()) && !is_dead)
  {
    move(game_time.deltaInSecs());
    handleSelectedItemMovement(game_time);

    ability_clock += game_time.deltaInSecs();
  }
  else if (is_dead)
  {
    handleRevive(game_time.deltaInSecs());
  }
  if (stun_timer != nullptr)
  {
    stun_timer->update(game_time);
  }
}

void CharacterBase::render(const ASGE::GameTime& game_time)
{
  bcw::Driver::getRenderer()->render(*sprite);
  handleSelectedItemRendering(game_time);
  if (stun_timer != nullptr)
  {
    stun_timer->render(game_time);
  }
}

void CharacterBase::heal(const int heal)
{
  int health_after_heal = health + heal;
  health                = health_after_heal > max_health ? max_health : health_after_heal;
}

void CharacterBase::takeDamage(const int damage)
{
  //  int health_after_damage = health - damage;
  //  health                  = health_after_damage > 0 ? health_after_damage : 0;

  health -= damage;
  if (health <= 0 && !is_dead)
  {
    onDie();

    is_dead = true;
    animator->setAnimation(DEATH_ANIMATION);
    animator->update({});
    is_visible = false;
  }
}

void CharacterBase::knockOut(const double time)
{
  if (!is_knocked_out)
  {
    is_knocked_out = true;
    knock_out_time += time;
    stun_timer = std::make_unique<Timer>();
    stun_timer->init({ sprite->xPos(), sprite->yPos() - 32.0F }, ASGE::COLOURS::LIME, time);
  }
}

void CharacterBase::revive(const bool reviving, const double duration)
{
  if (!is_dead)
  {
    return;
  }

  is_reviving = reviving;
  revive_time = duration;

  if (reviving)
  {
    revive_timer = bcw::Driver::getSceneHandler()->addGameObject<Timer>();
    revive_timer->init({ sprite->xPos(), sprite->yPos() }, ASGE::COLOURS::RED, duration);
  }
  else
  {
    bcw::Driver::getSceneHandler()->removeGameObject(revive_timer);
  }
}

void CharacterBase::pickUpItem(const std::shared_ptr<PickupableCollectable>& item)
{
  if (inventory.size() < INVENTORY_SIZE)
  {
    inventory.push_back(item);
    item->setAttachedToScene(this->shared_from_this());
    item->getSprite()->rotationInRadians(sprite->rotationInRadians());

    if (inventory.size() == 1)
    {
      selected_item = item;
    }
  }
}

bool CharacterBase::removeItem(
  const std::shared_ptr<PickupableCollectable>& item, const bool also_delete_item)
{
  auto it = std::find(inventory.begin(), inventory.end(), item);
  if (it != inventory.end())
  {
    inventory.erase(it);
    item->setIsPickedUp(false);

    // Just in case the hacker (who has invisibility powers) drops an item while invisible
    item->getSprite()->opacity(1);

    if (item == selected_item)
    {
      if (inventory.empty())
      {
        selected_item = nullptr;
      }
      else
      {
        selected_item = inventory.front();
      }
    }

    item->setAttachedToScene(nullptr);
    if (also_delete_item)
    {
      bcw::Driver::getSceneHandler()->removeGameObject(item);
    }

    return true;
  }

  return false;
}

ASGE::Sprite* CharacterBase::getSprite()
{
  return sprite.get();
}

void CharacterBase::ability(const bcw::Vector2& /*use_direction*/)
{
  // my ultimate is ready!
  resetAbilityTimer();
}

void CharacterBase::interact()
{
  // i n t e r a c t
  std::vector<bcw::Collider*> collisions =
    bcw::Driver::getCollisionHandler()->checkCollisionsInArea(
      sprite->xPos(), sprite->yPos(), interaction_range, interaction_range);

  for (const auto& collision : collisions)
  {
    std::shared_ptr<CharacterBase> character =
      std::dynamic_pointer_cast<CharacterBase>(collision->getAttachedScene());
    if (character != nullptr)
    {
      if (
        character->getTag() == getTag() && character->getIsDead() &&
        character != this->shared_from_this()) // no more self revive!
      {
        character->revive(true, 3.0);
        return;
      }
    }

    std::shared_ptr<IInteractable> interactable =
      std::dynamic_pointer_cast<IInteractable>(collision->getAttachedScene());
    if (interactable != nullptr)
    {
      interactable->onInteractionBegin();
    }
  }
}

void CharacterBase::onCollisionEnter(bcw::Collider* /*collision*/)
{
  // i've hit something!
}

void CharacterBase::onCollisionExit(bcw::Collider* /*collision*/)
{
  // goodbye, whatever I was colliding with...
}

void CharacterBase::useHeldItem(const bcw::Vector2& use_direction)
{
  if (selected_item != nullptr)
  {
    selected_item->onUseBegin(use_direction);
  }
}

void CharacterBase::dropHeldItem()
{
  // idk some dropping logic here if we want it
}

void CharacterBase::scrollInventory(const CharacterBase::ScrollDirection scroll_direction)
{
  if (selected_item == nullptr || inventory.size() == 1)
  {
    return;
  }

  auto it       = std::find(inventory.begin(), inventory.end(), selected_item);
  int next_item = static_cast<int>(std::distance(inventory.begin(), it)) +
                  (scroll_direction == CharacterBase::ScrollDirection::UP ? 1 : -1);

  if (next_item == -1)
  {
    next_item = static_cast<int>(inventory.size()) - 1;
  }
  else if (next_item == static_cast<int>(inventory.size()) - 1)
  {
    next_item = 0;
  }

  selected_item = inventory[static_cast<size_t>(next_item)];
}

void CharacterBase::resetAbilityTimer()
{
  ability_clock = 0.0;
}

bool CharacterBase::abilityReady() const
{
  return ability_cool_down <= ability_clock;
}

void CharacterBase::move(const double delta_time)
{
  // velocity.normalise(); as soon as you do this everything just dies???

  // bcw::Vector2 old_pos = { sprite->xPos(), sprite->yPos() };

  sprite->xPos(static_cast<float>(sprite->xPos() + velocity.x * speed * delta_time));
  sprite->yPos(static_cast<float>(sprite->yPos() + velocity.y * speed * delta_time));

  // Todo: Character rotation
  /* Also have to do this for the picked up item (handleSelectedItemMovement())
  if (old_pos != bcw::Vector2(sprite->xPos(), sprite->yPos()))
  {
    sprite->rotationInRadians(bcw::MoreMath::lookAt(old_pos, { sprite->xPos(), sprite->yPos() }));
  }
  */
}

bool CharacterBase::handleKnockOut(const double delta_time)
{
  if (!is_knocked_out)
  {
    return false;
  }

  knock_out_time -= delta_time;
  if (knock_out_time < 0.0)
  {
    is_knocked_out = false;
    knock_out_time = 0.0;
    stun_timer.reset();
  }

  return is_knocked_out;
}

void CharacterBase::handleRevive(const double delta_time)
{
  if (!is_reviving)
  {
    return;
  }

  revive_time -= delta_time;
  if (revive_time < 0.0)
  {
    onRevive();

    animator->setAnimation(IDLE_ANIMATION);
    animator->update({});

    health = max_health;

    is_dead     = false;
    is_visible  = true;
    is_reviving = false;
    revive_time = 0.0;
  }
}

void CharacterBase::handleSelectedItemMovement(const ASGE::GameTime& /*game_time*/)
{
  /*
    If the item isn't moved just before its rendered, then it ghosts behind the player
    The item is now moved in handleSelectedItemRendering

  if (selected_item != nullptr)
  {
    selected_item->forceMove(game_time, { sprite->xPos(), sprite->yPos() });
  }
   */
}

void CharacterBase::handleSelectedItemRendering(const ASGE::GameTime& game_time)
{
  if (selected_item != nullptr)
  {
    selected_item->forceMove(game_time, { sprite->xPos(), sprite->yPos() });
    // selected_item->forceRotate(game_time, {}); https://bit.ly/3khmlbS
    selected_item->forceRender(game_time);
  }
}

std::vector<std::shared_ptr<PickupableCollectable>> const& CharacterBase::getInventory() const
{
  return inventory;
}

ASGE::Sprite const* CharacterBase::getSprite() const
{
  return sprite.get();
}

bool CharacterBase::getIsDead() const
{
  return is_dead;
}

bool CharacterBase::getIsVisible() const
{
  return is_visible;
}

double CharacterBase::getAbilityClock() const
{
  return ability_clock;
}

bool CharacterBase::isKnockedOut() const
{
  return is_knocked_out;
}

void CharacterBase::setIsVisible(const bool visible)
{
  sprite->opacity(visible ? 1.0F : 0.25F); // m a g i c  n u m b e r
  is_visible = visible;
}

void CharacterBase::onDie()
{
  // Overridden in PlayerCharacter and NonPlayerCharacter
}

void CharacterBase::onRevive()
{
  // Overridden in PlayerCharacter
}
