// Created by Ben on 08/04/2022

#include "AlarmManager.hpp"

void AlarmManager::init()
{
  alarms                = bcw::Driver::getSceneHandler()->findGameObjects<Alarm>();
  non_player_characters = bcw::Driver::getSceneHandler()->findGameObjects<NonPlayerCharacter>();
}

void AlarmManager::update(const ASGE::GameTime& game_time)
{
  if (alarm_duration > 0.0)
  {
    alarm_duration -= game_time.deltaInSecs();

    if (alarm_duration < 0.0)
    {
      deactivate();
    }
  }
}

void AlarmManager::activate(const double duration, const AlarmManager::DurationMode duration_mode)
{
  switch (duration_mode)
  {
    case DurationMode::NEW:
      alarm_duration = duration;
      break;
    case DurationMode::ADDITION:
      alarm_duration += duration;
      if (alarm_duration > MAX_ALARM_DURATION)
      {
        alarm_duration = MAX_ALARM_DURATION;
      }
      break;
  }

  for (const auto& alarm : alarms)
  {
    alarm->activate(true);
  }

  for (const auto& npc : non_player_characters)
  {
    npc->alarmsActivated(true);
  }
}

void AlarmManager::deactivate()
{
  for (const auto& alarm : alarms)
  {
    alarm->activate(false);
  }

  for (const auto& npc : non_player_characters)
  {
    npc->alarmsActivated(false);
  }
}

void AlarmManager::toggle(const bool disable)
{
  for (const auto& alarm : alarms)
  {
    alarm->disable(disable);
  }
}
