#pragma once

#include "../glew/include/glew.h"
#include <array>
#include <memory>
#include <optional>
#include <string>

struct BmpImageData
{
  unsigned int dataPos; // Position in the file where the actual data begins
  unsigned int width, height;
  unsigned int imageSize; // = width*height*3
  // Actual RGB data
  std::shared_ptr<char[]> data;
};

std::optional<BmpImageData> loadBMP(std::string_view imagepath);