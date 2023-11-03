#pragma once

#include "OpenGLEntity.hpp"

class OpenGL3DEntity : public OpenGLEntity
{
public:
  virtual glm::vec3 getPosition() = 0;
  virtual void setPosition(glm::vec3 position) = 0;
};