#pragma once

#include "OpenGLEntity.hpp"

class OpenGL2DEntity : public OpenGLEntity
{
public:
  virtual glm::vec2 getPosition() = 0;
  virtual void setPosition(glm::vec2 position) = 0;
};