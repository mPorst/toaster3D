#include "SphereEntity.hpp"
#include "TextureLoading/loadBmp.hpp"
#include "helper/findFile.hpp"

#include <filesystem>
#include <iostream>
#include <random>

//#define DEBUG

namespace toaster
{

constexpr double PI = 3.1415926535;
constexpr int polarIterations = 8;
constexpr int azimuthalIterations = 8;

Sphere::Sphere(unsigned int entityId) : m_totalVertices(0), m_entityId(entityId), m_static(false) { setup(); }

Sphere::Sphere(unsigned int entityId, glm::vec3 position, double radius, bool isStatic)
    : m_totalVertices(0),   //
      m_entityId(entityId), //
      m_static(isStatic),   //
      m_radius(radius),     //
      m_position(position)
{
  setup();
}

Sphere::Sphere(unsigned int entityId, std::array<float, 3> position, double radius, bool isStatic)
    : m_totalVertices(0),   //
      m_entityId(entityId), //
      m_radius(radius),     //
      m_static(isStatic)
{
  m_position.x = position.at(0);
  m_position.y = position.at(1);
  m_position.z = position.at(2);
  setup();
}

GLuint Sphere::GetVertexBufferId() { return m_vertexBuffer; }

GLuint Sphere::GetColorBufferId() { return m_colorBuffer; }

GLuint Sphere::GetUvBufferId() { return m_uvBufferId; }

GLuint Sphere::GetTextureId() { return m_textureId; }

GLuint Sphere::GetVertexArrayId() { return m_vertexArrayID; }

unsigned int Sphere::getEntityId() { return m_entityId; }

GLuint Sphere::getVertices() { return m_vertexBufferRaw; }

void Sphere::setup()
{
  glGenVertexArrays(1, &m_vertexArrayID);
  glBindVertexArray(m_vertexArrayID);

  // sets m_vertexBuffer
  computeVertexBuffer(m_radius);

  glGenBuffers(1, &m_vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexBufferData.at(0)) * m_vertexBufferData.size(), m_vertexBufferData.data(),
               GL_DYNAMIC_DRAW);

  // glGenBuffers(1, &m_vertexBufferRaw);
  // glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferRaw);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices.at(0)) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

  const auto bmpdataOpt = loadBMP(helper::getFilepath("uvtemplate.bmp", helper::eFiletype::ASSET_FILETYPE));
  if (!bmpdataOpt.has_value())
  {
    std::cout << "NO BMP DATA !!!!!111einsienelf" << std::endl;
    assert(false);
  }

  auto bmpdata = bmpdataOpt.value();

  for (int i = 0; i < m_vertexBufferData.size();)
  {
    glm::vec3 vecToOrigin;
    vecToOrigin.x = -m_vertexBufferData.at(i);
    vecToOrigin.y = -m_vertexBufferData.at(i + 1);
    vecToOrigin.z = -m_vertexBufferData.at(i + 2);
    auto normOrigin = glm::normalize(vecToOrigin);
    GLfloat u = 0.5 + std::atan2(normOrigin.z, normOrigin.x) / (2.0 * PI);
    GLfloat v = 0.5 + std::asin(normOrigin.y) / (PI);
    m_uvData.push_back(u);
    m_uvData.push_back(v);

    assert(u > 0 && u < 1);
    assert(v > 0 && v < 1);

    i += 3;
  }

  glGenBuffers(1, &m_uvBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, m_uvBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_uvData.at(0)) * m_uvData.size(), m_uvData.data(), GL_DYNAMIC_DRAW);

  // Create one OpenGL texture
  glGenTextures(1, &m_textureId);

  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, m_textureId);

  // Give the image to OpenGL
  glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB, bmpdata.width, bmpdata.height, 0, GL_BGR, GL_UNSIGNED_BYTE, bmpdata.data.get());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // glGenBuffers(1, &m_colorBuffer);
  // glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(m_colorBufferData.at(0)) * m_colorBufferData.size(), m_colorBufferData.data(),
  // GL_DYNAMIC_DRAW);
  // glEnableVertexAttribArray(1);
  // glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
  // glVertexAttribPointer(
  //	1,                                // attribute. No particular reason for 1, but must match the layout in the
  // shader. 	3,                                // size 	GL_FLOAT,                         // type GL_FALSE,
  // // normalized? 	0,                                // stride 	(void*)0                          // array
  // buffer offset
  //);
}

void Sphere::setSpeed(glm::vec3 speed) { m_speed = speed; }

void Sphere::setPosition(glm::vec3 position) { m_position = position; }

glm::vec3 Sphere::getSpeed() { return m_speed; }

glm::vec3 Sphere::getPosition() { return m_position; }

void Sphere::computeVertexBuffer(double radius)
{
  std::vector<Vertex3D> meridianLine;
  double theta = 0;
  double phi = 0;
  for (int i = 0; i < polarIterations; i++)
  {
    m_vertices.push_back(meridianLine);
  }
  // set radius to 2
  // loop to walk along polar angle (theta)
  for (int i = 0; i < polarIterations; i++)
  {
    theta = static_cast<double>(i) * (180. / static_cast<double>(polarIterations - 1));
    // loop to walk along azimuthal angle
    // <= to "close" the sphere
    for (int j = 0; j <= azimuthalIterations; j++)
    {
      phi = static_cast<double>(j) * (360. / static_cast<double>(azimuthalIterations + 1));
      m_vertices.at(i).push_back(Vertex3D{0});
      m_vertices.at(i).at(j).x = (radius * cos(phi * 2 * PI / 360.) * sin(theta * 2 * PI / 360.));
      m_vertices.at(i).at(j).y = (radius * sin(phi * 2 * PI / 360.) * sin(theta * 2 * PI / 360.));
      m_vertices.at(i).at(j).z = (radius * cos(theta * PI / 180.));
      if (i == 0 || i == polarIterations - 1)
      {
        break;
      }
    }
  }

  // generate triangle: polar angle
  for (int i = 0; i < m_vertices.size() - 1; i++)
  {
    // need at least 1 "middle layer"
    for (int j = 0; j < m_vertices.at(1).size(); j++)
    {
      if (i == 0)
      {
#if defined(DEBUG)
        std::cout << i << ", " << j << std::endl;
#endif
        // top layer
        if (j == m_vertices.at(i + 1).size() - 1)
        {

          setVertexBufferEntry(i, 0);
          setVertexBufferEntry(i + 1, j);
          setVertexBufferEntry(i + 1, 0);
        }
        else
        {
          setVertexBufferEntry(i, 0);
          setVertexBufferEntry(i + 1, j);
          setVertexBufferEntry(i + 1, j + 1);
        }
        m_totalVertices += 3;
      }
      else if (i == m_vertices.size() - 2)
      {
// bottom layer
#if defined(DEBUG)
        std::cout << "At (i,j): " << i << ", " << j << std::endl;
#endif
        if (j == m_vertices.at(i).size() - 1)
        {
          setVertexBufferEntry(i, j);
          setVertexBufferEntry(i, 0);
          setVertexBufferEntry(i + 1, 0);
        }
        else
        {
          setVertexBufferEntry(i, j);
          setVertexBufferEntry(i, j + 1);
          setVertexBufferEntry(i + 1, 0);
        }
        m_totalVertices += 3;
      }
      else
      {
        if (i == polarIterations - 2)
        {
          break;
        }
        if (j == m_vertices.at(i).size() - 1)
        {

          setVertexBufferEntry(i, j);
          setVertexBufferEntry(i, 0);
          setVertexBufferEntry(i + 1, 0);

          setVertexBufferEntry(i, j);
          setVertexBufferEntry(i + 1, j);
          setVertexBufferEntry(i + 1, 0);
        }
        else
        {
          // middle layer
          setVertexBufferEntry(i, j);
          setVertexBufferEntry(i + 1, j);
          setVertexBufferEntry(i + 1, j + 1);

          setVertexBufferEntry(i, j);
          setVertexBufferEntry(i, j + 1);
          setVertexBufferEntry(i + 1, j + 1);
        }
        m_totalVertices += 6;
      }
    }
  }
  m_vertexBufferDataStatic = m_vertexBufferData;

#ifdef DEBUG
  std::cout << "Sphere total count of vertices: " << m_totalVertices << std::endl;
#endif
}

void Sphere::setVertexBufferEntry(int i, int j)
{
  m_vertexBufferData.push_back(m_vertices.at(i).at(j).x);
  m_vertexBufferData.push_back(m_vertices.at(i).at(j).y);
  m_vertexBufferData.push_back(m_vertices.at(i).at(j).z);
}

GLuint Sphere::getAmountVertices() { return m_totalVertices; }

std::vector<GLfloat> Sphere::GetVertexBufferData() { return m_vertexBufferData; }

} // namespace toaster