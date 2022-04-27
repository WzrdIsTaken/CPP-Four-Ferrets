// Created by Ben on 03/03/2022

#pragma once

#include "../../Component.hpp"

namespace bcw
{
  class [[maybe_unused]] FileStream : public bcw::Component
  {
   public:
    FileStream()           = default;
    ~FileStream() override = default;

    [[maybe_unused]] bool createFile(
      const std::string& file_path, const std::vector<std::string>& contents,
      bool override_existing_file = false);
    [[maybe_unused]] bool writeFile(
      const std::string& file_path, const std::vector<std::string>& contents,
      bool override_existing_file = false);

    [[maybe_unused]] [[nodiscard]] std::vector<std::string> readFile(const std::string& file_path);
    [[maybe_unused]] [[nodiscard]] std::vector<std::string> readCreateFile(
      const std::string& file_path, // Good name dude
      const std::vector<std::string>& contents);

    [[maybe_unused]] bool deleteFile(const std::string& file_path);
  };
} // namespace bcw