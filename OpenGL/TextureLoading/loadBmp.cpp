#include "loadBmp.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>


std::optional<BmpImageData> loadBMP(std::string_view imagepath)
{
  std::filesystem::path currentPath = std::filesystem::current_path();
  currentPath /= imagepath;
  std::cout << currentPath << std::endl;
  std::fstream file;
  file.open(currentPath.string(), std::ios::in | std::ios::binary);

  if (!file.is_open())
  {
    return std::nullopt;
  }

  BmpImageData bmpdata{0};
  std::array<char, 54> header; // Each BMP file begins by a 54-bytes header

  // size estimation via tellg is unsafe:
  // https://stackoverflow.com/questions/22984956/tellg-function-give-wrong-size-of-file/22986486#22986486
  file.ignore(std::numeric_limits<std::streamsize>::max());
  std::streamsize fileSize = file.gcount();
  file.seekg(0, std::ios_base::beg);
  file.read(header.data(), 54);
  if (file.gcount() != 54)
  {
    printf("Not a correct BMP file\n");
    return std::nullopt;
  }

  if (header[0] != 'B' || header[1] != 'M')
  {
    printf("Not a correct BMP file\n");
    return std::nullopt;
  }

  bmpdata.dataPos = *(int*)&(header[0x0A]);
  bmpdata.imageSize = *(int*)&(header[0x22]);
  bmpdata.width = *(int*)&(header[0x12]);
  bmpdata.height = *(int*)&(header[0x16]);

  if (bmpdata.imageSize == 0)
  {
    bmpdata.imageSize = bmpdata.width * bmpdata.height * 3;
  }

  // Create a buffer
  bmpdata.data = std::make_shared<char[]>(bmpdata.imageSize);

  // Read the actual data from the file into the buffer
  file.read(bmpdata.data.get(), bmpdata.imageSize);

  // Everything is in memory now, the file can be closed
  file.close();

  return bmpdata;
}