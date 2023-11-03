#include "Camera.h"
#include "glm/gtx/transform.hpp"
#include <iostream>

Camera::Camera()
    : m_fieldOfView(45.0f), m_width(4.0f), m_height(3.0f), m_nearClipping(0.1f), m_farClipping(10000.0f),
      m_verticalAngle(0), m_horizontalAngle(4.0)
{
  m_position = glm::vec3(0.0f, 0.0f, 0.0f);
  m_lookAt = glm::vec3(0.0f);
  m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
  m_viewMatrix = glm::lookAt(m_position, m_lookAt, m_upVector);

  m_projectionMatrix = glm::perspective(glm::radians(45.0f), m_width / m_height, m_nearClipping, m_farClipping);
}

glm::mat4 Camera::getViewMatrix() { return m_viewMatrix; }

glm::mat4 Camera::getProjectionMatrix() { return m_projectionMatrix; }

void Camera::update(float speed, float deltaTime, std::tuple<double, double> viewingAngles)
{
  float mouseSpeedup = 0.005f;
  if (speed != 0)
  {
    m_position += speed * deltaTime;
  }

  m_horizontalAngle = static_cast<float>(std::get<0>(viewingAngles));
  m_verticalAngle = static_cast<float>(std::get<1>(viewingAngles));

  // direction x right = up
  glm::vec3 direction(cos(m_verticalAngle) * sin(m_horizontalAngle), sin(m_verticalAngle),
                      cos(m_verticalAngle) * cos(m_horizontalAngle));

  glm::vec3 right = glm::vec3(sin(m_horizontalAngle - 3.14f / 2.0f), 0, cos(m_horizontalAngle - 3.14f / 2.0f));

  m_upVector = glm::cross(right, direction);

  m_viewMatrix = glm::lookAt(m_position, m_position + direction, m_upVector);

  m_projectionMatrix = glm::perspective(glm::radians(45.0f), m_width / m_height, m_nearClipping, m_farClipping);

  return;
}

void Camera::SetPos(std::array<float, 3> pos)
{
  m_position.x = pos.at(0);
  m_position.y = pos.at(1);
  m_position.z = pos.at(2);
}

std::array<float, 3> Camera::getPos() { return {m_position.x, m_position.y, m_position.z}; }

float Camera::getPhi() { return m_horizontalAngle; }

float Camera::getTheta() { return m_verticalAngle; }

void Camera::setPhi(float newPhi) { m_horizontalAngle = newPhi; }

void Camera::setTheta(float newTheta) { m_verticalAngle = newTheta; }