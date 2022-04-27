// Created by Ben on 25/02/2022

#pragma once

#include <Engine/FileIO.hpp>

#include <soloud.h>
#include <soloud_wav.h>

#include <map>
#include <string>

namespace bcw
{
  class AudioHandler
  {
   public:
    AudioHandler();
    ~AudioHandler();

    AudioHandler(const AudioHandler&) = delete;
    AudioHandler& operator=(const AudioHandler&) = delete;

    void update(const ASGE::GameTime& game_time);

    // A basic struct to store some audio data
    struct AudioData
    {
      ASGE::FILEIO::IOBuffer io_buffer; // doesn't like being stored as an unsigned char?
      unsigned int length = 0;          // TODO: work out why -
                                        // current solution means FileIO is included globally ;_;
      AudioData()  = default;
      ~AudioData() = default;
    };

    // Loads an audio track from a file
    [[maybe_unused]] AudioData loadAudio(const std::string& file_path);

    // Plays some audio from a filepath
    [[maybe_unused]] std::shared_ptr<SoLoud::Wav>
    playAudio(const std::string& file_path, float volume = 1.0F, bool loop = false);
    // Plays some audio from a loaded AudioData base
    [[maybe_unused]] std::shared_ptr<SoLoud::Wav>
    playAudio(AudioData& audio_data, float volume = 1.0F, bool loop = false);

    // Stops an audio clip
    [[maybe_unused]] bool stopSource(const std::shared_ptr<SoLoud::Wav>& track);
    // Stop all audio clips
    [[maybe_unused]] void stopAllSources();

   private:
    SoLoud::Soloud audio_engine;
    std::map<std::string, AudioData> audio_tracks;
    std::map<std::shared_ptr<SoLoud::Wav>, double> audio_sources; // TODO: object pool this?
  };
} // namespace bcw
