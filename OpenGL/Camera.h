#include "glm/glm.hpp"
#include <array>
#include <tuple>

class Camera
{
public:
  Camera();
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();
  void update(float speed, float deltaTime, std::tuple<double, double> mousePosRel);
  void SetPos(std::array<float, 3> pos);
  std::array<float, 3> getPos();
  float getPhi();
  float getTheta();
  void setPhi(float newPhi);
  void setTheta(float newTheta);

private:
  glm::vec3 m_position;
  glm::vec3 m_lookAt;
  glm::vec3 m_upVector;
  glm::mat4 m_viewMatrix;

  float m_horizontalAngle;
  float m_verticalAngle;

  glm::float32 m_fieldOfView;
  glm::float32 m_width, m_height;
  glm::float32 m_nearClipping, m_farClipping;
  glm::mat4 m_projectionMatrix;
};