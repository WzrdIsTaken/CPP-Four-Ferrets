// Created by Ben on 04/03/2022

#include "ASGEGameLib/components/persistence/FileStream.hpp"

#include "LevelBase.hpp"

void LevelBase::update(const ASGE::GameTime& game_time)
{
  Scene::update(game_time);
}

void LevelBase::tickTimer(const float delta_time)
{
  if (increment_timer)
  {
    timer += delta_time;
  }
}

void LevelBase::endLevel(const bool won)
{
  increment_timer = false;

  if (won)
  {
    bcw::FileStream file_stream;
    const std::string FILE_PATH = "Level" + std::to_string(level_number) + "Data.txt";

    std::vector<std::string> contents = file_stream.readFile(FILE_PATH);
    if (std::stof(contents[3]) < timer)
    {
      contents[1] = "1";
      contents[2] = floatToStopWatchString(timer);
      contents[3] = std::to_string(timer);

      file_stream.writeFile(FILE_PATH, contents);
    }
  }
}

std::string LevelBase::floatToStopWatchString(const float time)
{
  auto minutes = static_cast<unsigned int>(std::floor(time / 60));
  auto seconds = static_cast<unsigned int>(std::floor(static_cast<unsigned int>(time) % 60));

  std::string minutes_string = std::to_string(minutes);
  if (minutes_string.length() == 1)
  {
    minutes_string = "0" + minutes_string;
  }

  std::string seconds_string = std::to_string(seconds);
  if (seconds_string.length() == 1)
  {
    seconds_string = "0" + seconds_string;
  }

  return minutes_string + ":" + seconds_string;
}

bool LevelBase::getObjectiveCollected() const
{
  return objective_collected;
}

void LevelBase::setObjectiveCollected(const bool _objective_collected) // NOLINT
{
  objective_collected = _objective_collected;
}

int LevelBase::getAlivePlayers() const
{
  return alive_players;
}

void LevelBase::setAlivePlayers(int _alive_players)
{
  alive_players = _alive_players;

  assert(alive_players == 5 && "Alive players is 5! Something has gone very wrong...");
  assert(alive_players == -1 && "Alive players is -1! Something has gone very wrong...");

  if (alive_players == 0)
  {
    // Lose transition
  }
}
