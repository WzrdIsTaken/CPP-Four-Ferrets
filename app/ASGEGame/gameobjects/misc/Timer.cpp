// Created by Ben on 17/03/2022

#include "../../helper/ZOrderData.hpp"

#include "Timer.hpp"

void Timer::init(
  const bcw::Vector2& position, const ASGE::Colour& colour,
  const double _duration) // NOLINT
{
  duration = _duration;

  // clang tidy gives me the 'suspicious string' warning... it was the one that
  // formatted it!!!
  std::map<std::string, std::vector<std::string>> animations = { { "default",
                                                                   { "/data/images/timer/"
                                                                     "timer_0.png",
                                                                     "/data/images/timer/"
                                                                     "timer_1.png",
                                                                     "/data/images/timer/"
                                                                     "timer_2.png",
                                                                     "/data/images/timer/"
                                                                     "timer_3.png",
                                                                     "/data/images/timer/"
                                                                     "timer_4.png",
                                                                     "/data/images/timer/"
                                                                     "timer_5.png",
                                                                     "/data/images/timer/"
                                                                     "timer_6.png",
                                                                     "/data/images/timer/"
                                                                     "timer_7.png",
                                                                     "/data/images/timer/"
                                                                     "timer_8.png" } } };

  sprite = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    sprite.get(), animations["default"].front(), position);
  sprite->colour(colour);

  // Not really a collectable but one above map and one below player
  sprite->setGlobalZOrder(ZOrderData::COLLECTABLES);

  // this was 8 now 4 idk why 8 doesn't work anymore
  // wtf
  // ok 4 for release, 8 for testing??? idk dude
  animator = std::make_unique<bcw::Animator>(sprite.get(), animations, duration / 4.0);
}

void Timer::update(const ASGE::GameTime& game_time)
{
  duration -= game_time.deltaInSecs();

  if (animator->getCurrentFrame() == 8) // 10/10 code
  {
    sprite->opacity(0);
  }

  if (duration < 0.0)
  {
    bcw::Driver::getSceneHandler()->removeGameObject(this->shared_from_this());
  }
}

void Timer::render(const ASGE::GameTime& game_time)
{
  animator->update(game_time);

  bcw::Driver::getRenderer()->render(*sprite);
}
