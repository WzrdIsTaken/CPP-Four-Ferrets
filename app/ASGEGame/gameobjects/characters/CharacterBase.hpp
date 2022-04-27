// Created by Ben on 10/03/2022

#pragma once

#include "ASGEGameLib/Driver.hpp"
#include "ASGEGameLib/components/animator/Animator.hpp"
#include "ASGEGameLib/components/collision/Collider.hpp"

#include "../mapstuff/collectables/PickupableCollectable.hpp"
#include "../misc/Timer.hpp"

class CharacterBase : public bcw::Scene
{
 public:
  CharacterBase()           = default;
  ~CharacterBase() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;
  ASGE::Sprite* getSprite(); // ?? norbert

  virtual void heal(int heal);
  virtual void takeDamage(int damage);
  void knockOut(double time);
  void revive(bool reviving, double duration = 0.0); // bool so you can cancel the action

  void pickUpItem(const std::shared_ptr<PickupableCollectable>& item);
  bool removeItem(const std::shared_ptr<PickupableCollectable>& item, bool also_delete_item);

  std::vector<std::shared_ptr<PickupableCollectable>> const& getInventory() const;
  ASGE::Sprite const* getSprite() const;
  bool getIsDead() const;
  bool isKnockedOut() const;

  bool getIsVisible() const;
  void setIsVisible(bool visible); // Was originally not needed, but now we have bushes...

  virtual void ability(const bcw::Vector2& use_direction);
  virtual void interact();
  void useHeldItem(const bcw::Vector2& use_direction);
  void dropHeldItem();

 protected:
  virtual void onCollisionEnter(bcw::Collider* collision);
  virtual void onCollisionExit(bcw::Collider* collision);

  virtual void onDie();
  virtual void onRevive();

  bool abilityReady() const;
  double getAbilityClock() const;

  enum class ScrollDirection
  {
    UP,
    DOWN
  };
  void scrollInventory(ScrollDirection scroll_direction);

  std::unique_ptr<ASGE::Sprite> sprite;
  std::unique_ptr<bcw::Animator> animator;
  std::unique_ptr<bcw::Collider> collider;

  bcw::Vector2 velocity = bcw::Vector2(0.0F, 0.0F);

  // Stats
  int health               = 100;
  float speed              = 0;
  float interaction_range  = 70.0F;
  double ability_cool_down = 3.0;

  // Other super cool game mechanics
  bool is_visible = true; // will be eventually used for raycasting I hope :pray: james.......

  const std::string IDLE_ANIMATION  = "Idle";
  const std::string RUN_ANIMATION   = "Running";
  const std::string DEATH_ANIMATION = "Dead";

 private:
  void move(double delta_time);
  bool handleKnockOut(double delta_time);
  void handleRevive(double delta_time);

  void handleSelectedItemMovement(const ASGE::GameTime& game_time);
  void handleSelectedItemRendering(const ASGE::GameTime& game_time);

  void resetAbilityTimer();

  bool is_knocked_out   = false;
  double knock_out_time = 0.0;

  bool is_dead = false;

  bool is_reviving                    = false;
  double revive_time                  = 0.0;
  std::shared_ptr<Timer> revive_timer = nullptr;
  std::unique_ptr<Timer> stun_timer   = nullptr;

  int max_health       = 100;
  double ability_clock = 0;

  std::vector<std::shared_ptr<PickupableCollectable>> inventory{};
  std::shared_ptr<PickupableCollectable> selected_item;
  const size_t INVENTORY_SIZE = 5;
};
