// Created by Ben on 03/03/2022

#include <cstdio>
#include <fstream>

#include "../../../../include/ASGEGameLib/components/persistence/FileStream.hpp"

bool bcw::FileStream::createFile(
  const std::string& file_path, const std::vector<std::string>& contents,
  const bool override_existing_file)
{
  if (!std::ifstream(file_path) || override_existing_file)
  {
    std::ofstream new_file(file_path);
    for (const auto& line : contents)
    {
      new_file << line << std::endl;
    }
    new_file.close();

    return true;
  }

  return false;
}

bool bcw::FileStream::writeFile(
  const std::string& file_path, const std::vector<std::string>& contents,
  const bool override_existing_file)
{
  if (std::ifstream(file_path))
  {
    if (override_existing_file)
    {
      std::ofstream existing_file(file_path);
      for (const auto& line : contents)
      {
        existing_file << line << std::endl;
      }
      existing_file.close();
    }
    else
    {
      // soon™
    }

    return true;
  }

  return false;
}

std::vector<std::string> bcw::FileStream::readFile(const std::string& file_path)
{
  std::vector<std::string> lines;

  if (std::ifstream(file_path))
  {
    std::ifstream file_stream;
    file_stream.open(file_path);

    std::string line;
    while (std::getline(file_stream, line))
    {
      lines.push_back(line);
    }

    file_stream.close();
  }

  return lines;
}

std::vector<std::string> bcw::FileStream::readCreateFile(
  const std::string& file_path, const std::vector<std::string>& contents)
{
  std::vector<std::string> file_contents = readFile(file_path);
  if (file_contents.empty())
  {
    createFile(file_path, contents, true);
    file_contents = contents;
  }
  return file_contents;
}

bool bcw::FileStream::deleteFile(const std::string& file_path)
{
  if (std::ifstream(file_path))
  {
    remove(file_path.c_str()); // Probably a better way of doing this?
    return true;
  }

  return false;
}
