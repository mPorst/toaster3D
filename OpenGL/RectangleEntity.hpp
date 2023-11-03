#pragma once

#include "TextureLoading/loadBmp.hpp"
#include "entity/OpenGL2DEntity.hpp"
#include "glew/include/glew.h"
#include "glm/glm.hpp"
#include <array>
#include <chrono>
#include <vector>

namespace toaster
{
class Rectangle : public OpenGL2DEntity
{
public:
  Rectangle(unsigned int entityId);

  unsigned int getEntityId() override;
  GLuint GetVertexArrayId() override;
  GLuint GetVertexBufferId() override;
  GLuint GetColorBufferId() override;
  GLuint GetUvBufferId() override;
  GLuint GetTextureId() override;
  std::vector<GLfloat> GetVertexBufferData() override;
  void setTexture(BmpImageData bufferdata);

  // GLuint getVertexBuffer() override;
  GLuint getVertices();
  GLuint getAmountVertices() override;

  glm::vec2 getPosition() override;
  void setPosition(glm::vec2 position) override;

private:
  void setup();

  bool m_static;
  unsigned int m_entityId;
  GLuint m_vertexArrayID, m_vertexBuffer, m_vertexBufferRaw, m_colorBuffer, m_totalVertices, m_textureId, m_uvBufferId;
  std::vector<GLfloat> m_vertexBufferData;
  std::vector<GLfloat> m_vertexBufferDataStatic;
  std::vector<std::vector<Vertex2D>> m_vertices;
  std::vector<GLfloat> m_colorBufferData;
  std::vector<GLfloat> m_uvData;
  glm::vec2 m_position;
  GLfloat m_mass;
  double m_radius;
  std::chrono::high_resolution_clock::time_point m_timePoint;
};
} // namespace toaster