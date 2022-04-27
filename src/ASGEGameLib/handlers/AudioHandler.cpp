// Created by Ben on 25/02/2022

#include <Engine/FileIO.hpp>
#include <Engine/GameTime.hpp>

#include "../../../include/ASGEGameLib/handlers/AudioHandler.hpp"

bcw::AudioHandler::AudioHandler()
{
  audio_engine.init();
}

bcw::AudioHandler::~AudioHandler()
{
  audio_engine.deinit();
}

void bcw::AudioHandler::update(const ASGE::GameTime& game_time)
{
  for (auto it = audio_sources.cbegin(), next_it = it; it != audio_sources.cend(); it = next_it)
  {
    audio_sources[(*it).first] -= game_time.deltaInSecs();

    ++next_it;
    if (audio_sources[(*it).first] < 0.0F)
    {
      audio_sources.erase(it);
    }
  }
}

bcw::AudioHandler::AudioData bcw::AudioHandler::loadAudio(const std::string& file_path)
{
  if (audio_tracks.contains(file_path))
  {
    return audio_tracks[file_path];
  }

  AudioData data{};
  ASGE::FILEIO::File audio_file;
  if (audio_file.open(file_path))
  {
    data.io_buffer = audio_file.read();
    data.length    = static_cast<unsigned int>(data.io_buffer.length);
  }
  audio_file.close();

  return data;
}

std::shared_ptr<SoLoud::Wav>
bcw::AudioHandler::playAudio(const std::string& file_path, const float volume, const bool loop)
{
  AudioData audio_data = loadAudio(file_path);
  return playAudio(audio_data, volume, loop);
}

std::shared_ptr<SoLoud::Wav> bcw::AudioHandler::playAudio(
  bcw::AudioHandler::AudioData& audio_data, const float volume, const bool loop)
{
  std::shared_ptr<SoLoud::Wav> audio_source = std::make_shared<SoLoud::Wav>();

  audio_source->loadMem(
    audio_data.io_buffer.as_unsigned_char(),
    static_cast<unsigned int>(audio_data.io_buffer.length),
    false,
    false);
  audio_source->setVolume(volume);
  audio_source->setLooping(loop);

  audio_engine.play(*audio_source);
  audio_sources[audio_source] = audio_source->getLength();

  return audio_source;
}

bool bcw::AudioHandler::stopSource(const std::shared_ptr<SoLoud::Wav>& track)
{
  if (audio_sources.contains(track))
  {
    audio_sources[track] = -1;
    return true;
  }
  return false;
}

void bcw::AudioHandler::stopAllSources()
{
  for (const auto& source : audio_sources)
  {
    audio_sources[source.first] = -1;
  }
}
