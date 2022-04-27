// Created by Ben on 04/03/2022

#pragma once

#include "ASGEGameLib/Driver.hpp"

class LevelBase : public bcw::Scene
{
 public:
  void update(const ASGE::GameTime& game_time) override;

  bool getObjectiveCollected() const;
  void setObjectiveCollected(bool _objective_collected);

  int getAlivePlayers() const;
  void setAlivePlayers(int _alive_players);

 protected:
  void tickTimer(float delta_time);
  void endLevel(bool won);

  bool increment_timer = false;
  int level_number;

 private:
  std::string floatToStopWatchString(float time);

  int alive_players        = 4;
  float timer              = 0;
  bool objective_collected = false;
};
