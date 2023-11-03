#pragma once

#include "entity/OpenGL3DEntity.hpp"
#include "glew/include/glew.h"
#include "glm/glm.hpp"
#include <array>
#include <chrono>
#include <vector>

namespace toaster
{
class Sphere : public OpenGL3DEntity
{
public:
  Sphere(unsigned int entityId);
  Sphere(unsigned int entityId, glm::vec3 position, double radius, bool isStatic = true);
  Sphere(unsigned int entityId, std::array<float, 3> position, double radius, bool isStatic = true);

  unsigned int getEntityId() override;
  GLuint GetVertexArrayId() override;
  GLuint GetVertexBufferId() override;
  GLuint GetColorBufferId() override;
  GLuint GetUvBufferId() override;
  GLuint GetTextureId() override;
  std::vector<GLfloat> GetVertexBufferData() override;

  // GLuint getVertexBuffer() override;
  GLuint getVertices();
  GLuint getAmountVertices() override;

  void setSpeed(glm::vec3 speed);
  void setPosition(glm::vec3 position) override;

  glm::vec3 getSpeed();
  glm::vec3 getPosition() override;

private:
  void setup();
  void computeVertexBuffer(double radius = 3.);
  void setVertexBufferEntry(int i, int j);

  bool m_static;
  unsigned int m_entityId;
  GLuint m_vertexArrayID, m_vertexBuffer, m_vertexBufferRaw, m_colorBuffer, m_totalVertices, m_textureId, m_uvBufferId;
  std::vector<GLfloat> m_vertexBufferData;
  std::vector<GLfloat> m_vertexBufferDataStatic;
  std::vector<std::vector<Vertex3D>> m_vertices;
  std::vector<GLfloat> m_colorBufferData;
  std::vector<GLfloat> m_uvData;
  glm::vec3 m_speed;
  glm::vec3 m_position;
  GLfloat m_mass;
  double m_radius;
  std::chrono::high_resolution_clock::time_point m_timePoint;
};
} // namespace toaster