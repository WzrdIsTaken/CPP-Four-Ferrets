// Created by Ben on 03/03/2022.

#pragma once

#include "ASGEGameLib/Driver.hpp"

class MusicManager : public bcw::Scene
{
 public:
  MusicManager()           = default;
  ~MusicManager() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;

  void queueSongs(const std::vector<std::string>& songs);
  void playSong();

 private:
  void shuffleSongs();

  std::deque<std::string> music_queue;
  std::string first_song;

  double song_timer          = 0;
  double current_song_length = 0;
};
