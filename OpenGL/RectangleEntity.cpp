#include "RectangleEntity.hpp"
#include "TextureLoading/loadBmp.hpp"
#include "helper/findFile.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <random>

//#define DEBUG

namespace toaster
{
constexpr double PI = 3.1415926535;
constexpr int polarIterations = 8;
constexpr int azimuthalIterations = 8;

Rectangle::Rectangle(unsigned int entityId) : m_totalVertices(6), m_entityId(entityId), m_static(false) { setup(); }

GLuint Rectangle::GetVertexBufferId() { return m_vertexBuffer; }

GLuint Rectangle::GetColorBufferId() { return m_colorBuffer; }

GLuint Rectangle::GetUvBufferId() { return m_uvBufferId; }

GLuint Rectangle::GetTextureId() { return m_textureId; }

GLuint Rectangle::GetVertexArrayId() { return m_vertexArrayID; }

unsigned int Rectangle::getEntityId() { return m_entityId; }

GLuint Rectangle::getVertices() { return m_vertexBufferRaw; }

void Rectangle::setup()
{
  m_position = {0.0f, 0.0f};

  glGenVertexArrays(1, &m_vertexArrayID);
  glBindVertexArray(m_vertexArrayID);

  // sets m_vertexBuffer
  constexpr GLfloat recSize = 0.1;
  m_vertexBufferData = {
      -recSize, recSize, -recSize, -recSize, recSize, recSize, -recSize, -recSize, recSize, recSize, recSize, -recSize};

  glGenBuffers(1, &m_vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexBufferData.at(0)) * m_vertexBufferData.size(), m_vertexBufferData.data(),
               GL_DYNAMIC_DRAW);

  const auto bmpdataOpt = loadBMP(helper::getFilepath("uvtemplate.bmp", helper::eFiletype::ASSET_FILETYPE));
  if (!bmpdataOpt.has_value())
  {
    std::cout << "NO BMP DATA !!!!!111einsienelf" << std::endl;
    assert(false);
  }

  auto bmpdata = bmpdataOpt.value();

  m_uvData = {0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0};

  glGenBuffers(1, &m_uvBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, m_uvBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_uvData.at(0)) * m_uvData.size(), m_uvData.data(), GL_DYNAMIC_DRAW);

  // Create one OpenGL texture
  glGenTextures(1, &m_textureId);

  // "Bind" the newly created texture : all future texture functions will modify
  // this texture
  glBindTexture(GL_TEXTURE_2D, m_textureId);

  // Give the image to OpenGL
  glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB, bmpdata.width, bmpdata.height, 0, GL_BGR, GL_UNSIGNED_BYTE, bmpdata.data.get());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Rectangle::setPosition(glm::vec2 position) { m_position = position; }

glm::vec2 Rectangle::getPosition() { return glm::vec2(m_position); }

GLuint Rectangle::getAmountVertices() { return m_totalVertices; }

std::vector<GLfloat> Rectangle::GetVertexBufferData() { return m_vertexBufferData; }

void Rectangle::setTexture(BmpImageData bmpdata)
{
  // "Bind" the newly created texture : all future texture functions will modify
  // this texture
  glBindTexture(GL_TEXTURE_2D, m_textureId);

  // pixels are 8bit only -> read all color channels from there !
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);

  // Give the image to OpenGL
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RED, bmpdata.width, bmpdata.height, 0, GL_RED, GL_UNSIGNED_BYTE, bmpdata.data.get());
}
} // namespace toaster