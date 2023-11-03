#include "OpenGL/OpenGLRenderer.h"

int main(int argc, char** argv)
{
  std::shared_ptr<toaster::OpenGLRenderer> pRenderEngine = std::make_shared<toaster::OpenGLRenderer>();

  pRenderEngine->init();

  unsigned int sphereId = 123;
  pRenderEngine->createSphere({0.0, 0.0, 5.0}, ++sphereId, 1, false);
  unsigned int iteration = 0;
  while (iteration < 3e2)
  {
    pRenderEngine->run(16.0);
    ++iteration;
  }
}