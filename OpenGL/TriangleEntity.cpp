#include "TriangleEntity.h"
#include <iostream>

TriangleEntity::TriangleEntity(unsigned int entityId) { Init(entityId); }

TriangleEntity::~TriangleEntity() {}

bool TriangleEntity::Init(unsigned int entityId)
{
  m_entityId = entityId;

  m_afVertexBufferData.reserve(9);

  m_afVertexBufferData = {-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

  static const GLfloat g_vertex_buffer_data[] = {
      -1.0f, -1.0f, 0.0f, //
      1.0f,  -1.0f, 0.0f, //
      0.0f,  1.0f,  0.0f, //
  };

  // for(int i = 0; i < m_afVertexBufferData.size(); i++)
  //{
  //   std::cout << "buffer at " << i << " contains " << m_afVertexBufferData.at(i) << " ";
  // }

  glGenVertexArrays(1, &m_nVertexArrayId);
  glBindVertexArray(m_nVertexArrayId);

  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &m_nVertexBufferId);
  // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferId);
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, 36, &m_afVertexBufferData[0], GL_STATIC_DRAW);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  std::cout << "vertexBufferId: " << m_nVertexBufferId << std::endl;

  return true;
}

unsigned int TriangleEntity::getEntityId() { return m_entityId; }

GLuint TriangleEntity::GetVertexArrayId() { return m_nVertexArrayId; }

GLuint TriangleEntity::GetVertexBufferId() { return m_nVertexBufferId; }

std::vector<GLfloat> TriangleEntity::GetVertexBufferData() { return m_afVertexBufferData; }

unsigned int TriangleEntity::getAmountVertices() { return 3; }

glm::vec2 TriangleEntity::getPosition() { return glm::vec2{0, 0}; }

void TriangleEntity::setPosition(glm::vec2 position)
{
  std::cerr << "setPosition Not implemented for triangle";
  assert(false);
}