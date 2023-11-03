#pragma once

#include "glew/include/glew.h"
#include "glm/gtx/transform.hpp"

#include <vector>
#include <array>

class OpenGLEntity{
public:
  OpenGLEntity()
  {
    
  }
  
  ~OpenGLEntity()
  {
    
  }

  virtual unsigned int getEntityId() = 0;
  virtual GLuint GetVertexArrayId() = 0;
  virtual GLuint GetVertexBufferId() = 0;
  virtual GLuint GetColorBufferId() = 0;
  virtual GLuint GetUvBufferId() = 0;
  virtual GLuint GetTextureId() = 0;
  virtual std::vector<GLfloat> GetVertexBufferData() = 0;
  virtual unsigned int getAmountVertices() = 0;
  
  
};

struct Vertex3D
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct Vertex2D
{
	GLfloat x;
	GLfloat y;
};