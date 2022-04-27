// Created by Ben on 08/04/2022

#pragma once

#include <ASGEGameLib/Driver.hpp>

#include "../../characters/NonPlayerCharacter.hpp"
#include "Alarm.hpp"

class AlarmManager : public bcw::Scene
{
 public:
  AlarmManager()           = default;
  ~AlarmManager() override = default;

  void update(const ASGE::GameTime& game_time) override;

  // IMPORTANT: Call after all the alarms and npcs have been added!
  void init();

  enum class DurationMode
  {
    NEW,
    ADDITION
  };
  void activate(double duration, DurationMode duration_mode);
  void deactivate();
  void toggle(bool disable);

 private:
  std::vector<std::shared_ptr<Alarm>> alarms;
  std::vector<std::shared_ptr<NonPlayerCharacter>> non_player_characters;

  double alarm_duration           = -1.0;
  const double MAX_ALARM_DURATION = 30.0;
};
