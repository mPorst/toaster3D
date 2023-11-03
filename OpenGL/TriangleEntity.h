#include "entity/OpenGL2DEntity.hpp"
#include "glew/include/glew.h"


class TriangleEntity : public OpenGL2DEntity
{
public:
  TriangleEntity(unsigned int entityId);
  ~TriangleEntity();

  unsigned int getEntityId() override;
  GLuint GetVertexArrayId() override;
  GLuint GetVertexBufferId() override;
  std::vector<GLfloat> GetVertexBufferData() override;
  unsigned int getAmountVertices() override;
  glm::vec2 getPosition() override;
  void setPosition(glm::vec2 position) override;

private:
  unsigned int m_entityId;
  GLuint m_nVertexArrayId;
  GLuint m_nVertexBufferId;
  std::vector<GLfloat> m_afVertexBufferData;

  bool Init(unsigned int entityId);
};