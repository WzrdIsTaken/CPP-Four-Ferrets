// Created by Ben on 10/03/2022

// ZOrder
#include "../../helper/ZOrderData.hpp"

// Header
#include "NonPlayerCharacter.hpp"

void NonPlayerCharacter::start()
{
  // Init base class
  CharacterBase::start();

  // Attributes
  DEFAULT_SPEED = speed;

  // Raycaster
  raycaster = std::make_unique<bcw::Raycast>();

  // Exclamation (like the little ! or ? above an NPC's head) sprite
  exclamation_sprite = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    exclamation_sprite.get(), "/data/images/guard/questioning.png", { 0, 0 }, 0.5F);
  exclamation_sprite->opacity(0.0F);
  // exclamation_sprite->opacity(ZOrderData::POP_UPS); phantom variable

  // Tag
  setTag("NPC");
}

void NonPlayerCharacter::init(const bcw::Vector2& spawn_position, MapManager* _path_finder)
{
  // Start position
  sprite->xPos(spawn_position.x);
  sprite->yPos(spawn_position.y);

  // Assignments
  path_finder = _path_finder;

  // Assign a path so back() isn't called on an empty deque
  npc_state = NpcState::DEFAULT;
  calculatePath();
}

void NonPlayerCharacter::update(const ASGE::GameTime& game_time)
{
  // Tick base class
  CharacterBase::update(game_time);

  // If the NPC isn't dead handle pathfinding, rotation and the exclamation_sprite's position
  if (!getIsDead())
  {
    handlePathFinding(game_time.deltaInSecs());

    // This is an... interesting..? work around to make the rotations play nice
    if (!idling)
    {
      rotation = velocity;
    }
    sprite->rotationInRadians(atan2(rotation.x, -rotation.y));

    exclamation_sprite->xPos(sprite->xPos() - EXCLAMATION_SPRITE_OFFSET.x);
    exclamation_sprite->yPos(sprite->yPos() - EXCLAMATION_SPRITE_OFFSET.y);
  }
}

void NonPlayerCharacter::render(const ASGE::GameTime& game_time)
{
  // Tick base class
  CharacterBase::render(game_time);

  // Render the exclamation sprite. Its visibility is toggled by its opacity attribute
  bcw::Driver::getRenderer()->render(*exclamation_sprite);
}

void NonPlayerCharacter::setPatrolRoute(
  const std::vector<bcw::Vector2>& _patrol_route, const PatrolDirection start_patrol_direction)
{
  // Set the patrol route, direction and calculate an intial path
  patrol_route     = _patrol_route;
  patrol_direction = start_patrol_direction;
  calculatePath();
}

void NonPlayerCharacter::changeNpcState(
  const NonPlayerCharacter::NpcState new_state, const std::shared_ptr<CharacterBase>& new_target)
{
  // Reset the NPC's path and assign new values
  path.clear();
  npc_state = new_state;
  target    = new_target;

  // The NPC shouldn't idle if chasing or fleeing
  switch (npc_state)
  {
    case NpcState::DEFAULT:
      idle_time = 1.0;
      break;
    case NpcState::CHASING:
      idle_time = 0.0;
      break;
    case NpcState::FLEEING:
      idle_time = 0.0;
      break;
  }

  // Calculate a new path based off the new values
  calculatePath();
}

void NonPlayerCharacter::handlePathFinding(const double delta_time)
{
  if (path.empty()) // because the calculatePath function is bot
  {
    return;
  }

  // If the NPC isn't idling, move between the path nodes
  if (!idling)
  {
    bcw::Vector2 current_position = { sprite->xPos(), sprite->yPos() };
    if (current_position.distance(path.back()) < SENSITIVITY)
    {
      animator->setAnimation(RUN_ANIMATION);
      path.pop_back();

      if (path.empty())
      {
        calculatePath();

        animator->setAnimation(IDLE_ANIMATION);
        velocity = { 0, 0 };
        idling   = true;
        return;
      }
    }

    bcw::Vector2 move_direction;
    move_direction.x = path.back().x - current_position.x;
    move_direction.y = path.back().y - current_position.y;
    move_direction.normalise();

    velocity = move_direction;
  }
  else
  {
    idle_time_timer += delta_time;
    if (idle_time_timer > idle_time)
    {
      idle_time_timer = 0.0;
      idling          = false;
    }
  }
}

void NonPlayerCharacter::calculatePath()
{
  // Just make sure the new target isn't the same as the old one! Or nasty error

  switch (npc_state)
  {
    case NpcState::DEFAULT:
    {
      /* Temporary!
      target_position = bcw::Vector2(
        static_cast<float>(rand() % 1280), // NOLINT
        static_cast<float>(rand() % 720)); // NOLINT
      */

      // Not so temporary
      if (!patrol_route.empty()) // don't want to have a bunch of unnecessary branches
      {
        if (patrol_direction == PatrolDirection::FORWARD)
        {
          calculatePatrolRoutePath(PatrolDirection::BACKWARD);
        }
        else
        {
          calculatePatrolRoutePath(PatrolDirection::FORWARD);
        }
      }
      break;
    }
    case NpcState::CHASING:
    {
      ASGE::Sprite const* target_sprite = target->getSprite();
      bcw::Vector2 target_position      = { target_sprite->xPos(), target_sprite->yPos() };

      // TODO: Once this is tile-based, we can just take like the first tile of the path
      // and then only append them to the path. This way the guard will follow the player
      // accurately and not pace it through walls. However, this isn't the ideal solution
      // as it might lag because lots of paths are being calculated per frame.

      // path = path_finder->findPath({ sprite->xPos(), sprite->yPos() }, target_position);
      path = { target_position };
      break;
    }
    case NpcState::FLEEING:
    {
      // not sure of the best way of doing this. have to move the npc away from the target
      // which is easy enough, however we also don't this path to be, e.g., inside a wall

      // [--> DON'T WORRY ABOUT THIS - I DON'T THINK IT WILL ACTUALLY BE NEEDED <--]
      break;
    }
  }
}

void NonPlayerCharacter::calculatePatrolRoutePath(const PatrolDirection _patrol_direction) // NOLINT
{
  bcw::Vector2 from_pos = { sprite->xPos(), sprite->yPos() };

  for (const auto& point : patrol_route)
  {
    std::deque<bcw::Vector2> part_of_path = path_finder->findPath(from_pos, point);
    for (const auto& part : part_of_path)
    {
      path.push_front(part);
    }

    from_pos = point;

    // path.push_back(point);
  }

  patrol_direction = _patrol_direction;
}

void NonPlayerCharacter::alarmsActivated(const bool activated)
{
  // Increase the guards speed if they are not chasing the player
  // If they are chasing, then they have their own speed modifier
  if (npc_state != NpcState::CHASING)
  {
    speed = activated ? DEFAULT_SPEED * ALARM_RAISED_SPEED_MODIFIER
                      : DEFAULT_SPEED * NORMAL_SPEED_MODIFIER;
  }
}

void NonPlayerCharacter::onDie()
{
  // If the NPC dies disable the exclamation sprite
  exclamation_sprite->opacity(0.0F);
}
