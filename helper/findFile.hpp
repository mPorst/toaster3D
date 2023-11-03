#pragma once

#include <filesystem>
#include <iostream>
#include <string>

namespace toaster::helper
{
constexpr std::string_view ASSET_FOLDER = "assets";
constexpr std::string_view SHADER_FOLDER = "shaders";

enum class eFiletype
{
  ASSET_FILETYPE, // texture, font
  SHADER_FILETYPE
};

/// @brief assumes that the binary path is one level above the root
/// @param filename filename to find
/// @param e_type type of file
/// @return the filepath
inline std::string getFilepath(std::string_view filename, eFiletype e_type)
{
  std::filesystem::path rootPath = std::filesystem::current_path().parent_path();
  std::filesystem::path& fullPath = rootPath;

  switch (e_type)
  {
    case eFiletype::ASSET_FILETYPE: //
    {
      fullPath.append(ASSET_FOLDER).append(filename);
      break;
    }
    case eFiletype::SHADER_FILETYPE: //
    {
      fullPath.append(SHADER_FOLDER).append(filename);
      break;
    }
  }

  if (!std::filesystem::exists(fullPath))
  {
    std::cerr << "file " << filename << " could not be found" << std::endl;
    return "";
  }
  else
  {
    return fullPath.string();
  }
}
} // namespace toaster::helper
