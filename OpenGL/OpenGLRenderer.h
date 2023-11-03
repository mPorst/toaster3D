#include "../IRenderer.h"
#include "Camera.h"
#include "TextureLoading/loadBmp.hpp"
#include "entity/OpenGL2DEntity.hpp"
#include "entity/OpenGL3DEntity.hpp"
#include "entity/OpenGLEntity.hpp"

//#include "glew.h"
#include "GLFW/glfw3.h"
#include "glew/include/glew.h"


// freetype
#include "external/freetype/include/ft2build.h"
#include FT_FREETYPE_H

#include <array>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace toaster
{
class OpenGLRenderer : public IRenderer
{
public:
  OpenGLRenderer(/* args */);
  ~OpenGLRenderer();

  void run(float deltaTime) override;

  bool init() override;
  bool isInitialized() override;

  void createSphere(std::array<float, 3> sphere, unsigned int sphereId, double radius, bool isStatic = true);
  void destroySphere(unsigned int sphereId);

  void quit() override;

  bool shouldClose() override;

  bool registerNew3DEntity(std::shared_ptr<OpenGL3DEntity> pEntity);

  void handleInput();

private:
  // variables
  GLFWwindow* m_window;
  bool m_bInitialized;
  GLuint m_programId;
  GLuint m_textProgramId;
  Camera m_camera;

  // window parameters
  int m_windowWidth, m_windowHeight;

  std::tuple<double, double> m_MousePosition;
  std::tuple<double, double> m_MousePositionRelative;

  std::tuple<double, double> m_rotationData;

  // init functions
  bool initWindow();
  // init freetype library for font rendering
  bool initFT();
  bool registerCallbacks();

  // callback registration
  static void error_callback(int error, const char* description);
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void mouse_callback(GLFWwindow* window, int button, int action, int mods);

  void handleMousePos();
  void resetMousePos();
  void getMessages();
  void handleMessages();

  void render3D();
  void render2D();

  // freetype glyph-related functions
  static bool loadGlyph(FT_Face& face, char charcode);
  static BmpImageData getGlyphTexture(FT_Face& face);

  // misc functions
  void printOglVersion();

  // error handling
  static void printNotInitializedError(std::string callingFunction);

  std::list<OpenGLEntity*> m_entities;
  std::list<std::shared_ptr<OpenGL3DEntity>> m_entitiesSharedPtr;
  std::list<std::shared_ptr<OpenGL2DEntity>> m_entitiesGuiSharedPtr;
  FT_Library m_ftLibrary;
  FT_Face m_ftFace;
};

} // namespace toaster