#include "OpenGLRenderer.h"
#include "RectangleEntity.hpp"
#include "SphereEntity.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include "helper/findFile.hpp"
#include "loadShaders.hpp"
#include <filesystem>

#if defined(_WIN32)
#include <source_location>
#elif defined(__linux__)
#include <experimental/source_location>
#endif
#include <algorithm>
#include <iostream>
#include <mutex>
#include <thread>
#include <tuple>

namespace toaster
{

OpenGLRenderer::OpenGLRenderer(/* args */) : m_bInitialized(false), m_windowWidth(1280), m_windowHeight(720) {}

OpenGLRenderer::~OpenGLRenderer() {}

bool OpenGLRenderer::registerNew3DEntity(std::shared_ptr<OpenGL3DEntity> pEntity)
{
  m_entitiesSharedPtr.push_back(pEntity);

  return true;
}

/// @brief init window and load shaders
/// @return
bool OpenGLRenderer::init()
{
  m_bInitialized = initWindow() && registerCallbacks() && initFT();
  m_camera = Camera();

  m_programId = LoadShaders(
      helper::getFilepath("SimpleVertexShader.vertexshader", helper::eFiletype::SHADER_FILETYPE).c_str(),
      helper::getFilepath("SimpleFragmentShader.fragmentshader", helper::eFiletype::SHADER_FILETYPE).c_str());
  m_textProgramId = LoadShaders(
      helper::getFilepath("TextVertexShader.vertexshader", helper::eFiletype::SHADER_FILETYPE).c_str(),
      helper::getFilepath("TextFragmentShader.fragmentshader", helper::eFiletype::SHADER_FILETYPE).c_str());

  if (m_programId == 0)
  {
    m_bInitialized = false;
  }
  // ensure the camera does not yeet away due to initial mouse position
  resetMousePos();

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  return m_bInitialized;
}

/// @brief main run-function of OpenGLRenderer
/// @param deltaTime the delta time of the whole mainLoop that passed in the
/// last iteration
void OpenGLRenderer::run(float deltaTime)
{
  // render stuff
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (m_entitiesGuiSharedPtr.empty())
  {
    std::shared_ptr<Rectangle> myRec = std::make_shared<Rectangle>(123);
    myRec->setPosition({0.0, 0.0});
    loadGlyph(m_ftFace, 'a');
    myRec->setTexture(getGlyphTexture(m_ftFace));
    m_entitiesGuiSharedPtr.push_back(myRec);
  }

  render3D();
  render2D();

  // std::this_thread::sleep_for(std::chrono::milliseconds(5));
  glfwSwapBuffers(m_window);
  // poll events required so window doesn't freeze
  glfwPollEvents();
  handleInput();
}

bool OpenGLRenderer::isInitialized() { return m_bInitialized; }

void OpenGLRenderer::createSphere(std::array<float, 3> pos, unsigned int sphereId, double radius, bool isStatic)
{
  std::shared_ptr<Sphere> spherePtr = std::make_shared<Sphere>(sphereId, pos, radius, isStatic);
  registerNew3DEntity(spherePtr);
}

void OpenGLRenderer::destroySphere(unsigned int id)
{
  if (!m_entitiesSharedPtr.empty())
  {
    auto it = std::find_if(
        m_entitiesSharedPtr.begin(), m_entitiesSharedPtr.end(), [&id](std::shared_ptr<OpenGLEntity> x) {
          return x->getEntityId() == id;
        });
    if (it != m_entitiesSharedPtr.end())
    {
      m_entitiesSharedPtr.erase(it++);
    }
  }
}

void OpenGLRenderer::quit()
{
  std::cout << "Reached end of application, terminating GLFW..." << std::endl;
  glfwDestroyWindow(m_window);
  glfwTerminate();
  return;
}

bool OpenGLRenderer::shouldClose() { return glfwWindowShouldClose(m_window); }

void OpenGLRenderer::handleInput()
{
  handleMousePos();
  resetMousePos();
}

/// @brief Get the cursor position
void OpenGLRenderer::handleMousePos()
{
  glfwGetCursorPos(m_window, &std::get<0>(m_MousePosition), &std::get<1>(m_MousePosition));

  // get only diff pos from center
  // this works only when the mouse position is reset after handleMousePos
  m_MousePositionRelative = m_MousePosition;
  std::get<0>(m_MousePositionRelative) -= m_windowWidth / 2;
  std::get<1>(m_MousePositionRelative) -= m_windowHeight / 2;
}

/// @brief Reset the mouse position to window center coordinates
void OpenGLRenderer::resetMousePos() { glfwSetCursorPos(m_window, m_windowWidth / 2, m_windowHeight / 2); }

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------ private ---------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------

// ----- init functions -----

bool OpenGLRenderer::initWindow()
{

  if (!glfwInit())
  {
    std::cout << "Initialization failed !" << std::endl;
    return false;
  }

  std::cout << "GLFW initalization done" << std::endl;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "My Title", NULL, NULL);

  if (!m_window)
  {
    std::cerr << "Could not create window. Fatal" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(m_window);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  // gl stuff
  int width, height;
  glfwGetFramebufferSize(m_window, &width, &height);
  glViewport(0, 0, width, height);

  return true;
}

bool OpenGLRenderer::initFT()
{
  bool initSuccessful = true;
  FT_Error error = FT_Init_FreeType(&m_ftLibrary);
  if (error > 0)
  {
    std::cerr << "Freetype could not be initialized. Font rendering won't be "
                 "possible."
              << std::endl;
    initSuccessful = false;
  }

  std::string fontFilename = "LucidaBrightRegular.ttf";
  fontFilename = helper::getFilepath(fontFilename, helper::eFiletype::ASSET_FILETYPE);
  error = FT_New_Face(m_ftLibrary, fontFilename.c_str(), 0, &m_ftFace);
  if (error == FT_Err_Unknown_File_Format)
  {
    std::cerr << "Could not open " << fontFilename << std::endl;
    initSuccessful = false;
  }
  else if (error != 0)
  {
    std::cerr << "Something went wrong reading file " << fontFilename << std::endl;
    initSuccessful = false;
  }

  error = FT_Set_Char_Size(m_ftFace, /* handle to face object         */
                           0,        /* char_width in 1/64 of points  */
                           16 * 64,  /* char_height in 1/64 of points */
                           300,      /* horizontal device resolution  */
                           300);     /* vertical device resolution    */

  if (error != 0)
  {
    std::cerr << "Could not set font size" << std::endl;
    initSuccessful = false;
  }

  return initSuccessful;
}

bool OpenGLRenderer::registerCallbacks()
{
  glfwSetErrorCallback(OpenGLRenderer::error_callback);
  glfwSetKeyCallback(m_window, OpenGLRenderer::key_callback);
  glfwSetMouseButtonCallback(m_window, OpenGLRenderer::mouse_callback);
  return true;
}

// ----- render functions -----

void OpenGLRenderer::render3D()
{
  glUseProgram(m_programId);

  m_camera.update(0.0, 16.0, m_rotationData);

  // Get a handle for our "myTextureSampler" uniform
  const GLuint samplerId = glGetUniformLocation(m_programId, "myTextureSampler");

  GLuint mvpId = glGetUniformLocation(m_programId, "MVP");
  glm::mat4 view = m_camera.getViewMatrix();
  glm::mat4 projection = m_camera.getProjectionMatrix();
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 mvp = projection * view * model;
  glUniformMatrix4fv(mvpId, 1, GL_FALSE, &mvp[0][0]);

  for (auto const& entity : m_entitiesSharedPtr)
  {
    if (entity == nullptr)
    {
      std::cout << "nullptr item in OpenGL Entity List !!" << std::endl;
      continue;
    }

    // glActiveTexture(GL_TEXTURE0);
    GLuint texId = entity->GetTextureId();
    glBindTexture(GL_TEXTURE_2D, texId);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(samplerId, 0);

    const GLuint translationId = glGetUniformLocation(m_programId, "translation");
    glm::mat4 translation = glm::translate(entity->getPosition());
    glUniformMatrix4fv(translationId, 1, GL_FALSE, &translation[0][0]);

    const GLuint rotationId = glGetUniformLocation(m_programId, "rotation");
    glm::mat4 rotation = glm::mat4(1.0f);
    glUniformMatrix4fv(rotationId, 1, GL_FALSE, &rotation[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, entity->GetVertexBufferId());
    glVertexAttribPointer(0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,        // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          (void*)0  // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, entity->GetUvBufferId());
    glVertexAttribPointer(1,        // attribute. No particular reason for 1, but must match the layout in the shader.
                          2,        // size : U+V => 2
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          (void*)0  // array buffer offset
    );

    // Draw
    size_t size = entity->GetVertexBufferData().size();
    unsigned int amountVertices = entity->getAmountVertices();
    glDrawArrays(GL_TRIANGLES, 0, entity->getAmountVertices()); // Starting from vertex 0
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
  }
}

void OpenGLRenderer::render2D()
{
  glUseProgram(m_textProgramId);

  // Get a handle for our "myTextureSampler" uniform
  const GLuint samplerId = glGetUniformLocation(m_textProgramId, "myTextureSampler");

  for (auto const& entity : m_entitiesGuiSharedPtr)
  {
    if (entity == nullptr)
    {
      std::cout << "nullptr item in OpenGL Entity List !!" << std::endl;
      continue;
    }

    entity->setPosition({0.3f, 0.3f});

    // glActiveTexture(GL_TEXTURE0);
    GLuint texId = entity->GetTextureId();
    glBindTexture(GL_TEXTURE_2D, texId);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(samplerId, 0);

    const GLuint translationId = glGetUniformLocation(m_textProgramId, "translation");
    // translation matrix still acts on 3d vector
    glm::vec3 pos = {entity->getPosition().x, entity->getPosition().y, 0.0f};
    glm::mat4 translation = glm::translate(pos);
    glUniformMatrix4fv(translationId, 1, GL_FALSE, &translation[0][0]);

    // uniform to translation matrix

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, entity->GetVertexBufferId());
    glVertexAttribPointer(0,        // attribute 0. No particular reason for 0, but
                                    // must match the layout in the shader.
                          2,        // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          (void*)0  // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, entity->GetUvBufferId());
    glVertexAttribPointer(1,        // attribute. No particular reason for 1, but must
                                    // match the layout in the shader.
                          2,        // size : U+V => 2
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          (void*)0  // array buffer offset
    );

    // Draw
    size_t size = entity->GetVertexBufferData().size();
    unsigned int amountVertices = entity->getAmountVertices();
    glDrawArrays(GL_TRIANGLES, 0, entity->getAmountVertices()); // Starting from vertex 0
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
  }
}

// ----- callbacks -----

void OpenGLRenderer::error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
  return;
}

void OpenGLRenderer::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) { return; }

void OpenGLRenderer::mouse_callback(GLFWwindow* window, int button, int action, int mods) { return; }

// ----- freetype related functions -----

bool OpenGLRenderer::loadGlyph(FT_Face& face, char charcode)
{
  bool loadSuccessful = false;
  bool renderSuccessful = false;
  unsigned int glyphIndex = FT_Get_Char_Index(face, charcode);
  FT_Error loadError = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
  FT_Error renderError = 0;
  if (loadError == 0)
  {
    loadSuccessful = true;
    renderError = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    if (renderError == 0)
    {
      renderSuccessful = true;
    }
  }

  return loadSuccessful && renderSuccessful;
}

BmpImageData OpenGLRenderer::getGlyphTexture(FT_Face& face)
{
  BmpImageData imgData;
  imgData.height = face->glyph->bitmap.rows;
  imgData.width = face->glyph->bitmap.width;
  const unsigned int imgSize = imgData.height * imgData.width;
  imgData.data = std::make_shared<char[]>(imgSize);
  memcpy(imgData.data.get(), face->glyph->bitmap.buffer, imgSize * sizeof(char));
  return imgData;
}

// ----- misc functions -----

void OpenGLRenderer::printOglVersion()
{
  if (!m_bInitialized)
  {
#if defined(_WIN32)
    printNotInitializedError(std::source_location::current().function_name());
#else
    printNotInitializedError(std::experimental::source_location::current().function_name());
#endif
  }

  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION);   // version as a string
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);
  return;
}

// ----- error handling -----

void OpenGLRenderer::printNotInitializedError(std::string callingFunction)
{
  std::cout << "Error: OpenGL is not yet initialized but function " << callingFunction << " was called" << std::endl;
  return;
}

} // namespace toaster