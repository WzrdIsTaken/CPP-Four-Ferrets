// Created by Ben on 03/03/2022

#include <algorithm>
#include <random>

#include "MusicManager.h"

void MusicManager::start()
{
  setDontDestroyOnLoad(true);
  setTag("MusicManager");
}

void MusicManager::update(const ASGE::GameTime& game_time)
{
  song_timer += game_time.deltaInSecs();
  if (song_timer > current_song_length)
  {
    song_timer = 0;
    playSong();
  }
}

void MusicManager::queueSongs(const std::vector<std::string>& songs)
{
  music_queue = std::deque<std::string>(songs.begin(), songs.end());
  shuffleSongs();
}

void MusicManager::shuffleSongs()
{
  std::random_device rd;
  std::mt19937 g(rd());

  std::shuffle(music_queue.begin(), music_queue.end(), g);
  // first_song = music_queue.back(); // magic
}

void MusicManager::playSong()
{
  if (first_song == music_queue.front())
  {
    std::string prev_first_song = music_queue.front();
    music_queue.pop_front();

    shuffleSongs();
    music_queue.push_back(prev_first_song);
  }

  std::string song_path                  = music_queue.front();
  bcw::AudioHandler::AudioData song_data = bcw::Driver::getAudioHandler()->loadAudio(song_path);

  std::shared_ptr<SoLoud::Wav> song =
    bcw::Driver::getAudioHandler()->playAudio(song_data, 0.25F, false);
  current_song_length = song->getLength();

  music_queue.pop_front();
  music_queue.push_back(song_path);
}
