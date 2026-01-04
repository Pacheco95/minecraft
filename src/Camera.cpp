#include "Camera.h"

#include "Container.h"

namespace App {

Camera::Camera(const glm::vec3 &position)
    : m_position(position), m_initialPosition(m_position), m_front(0.0f, 0.0f, -1.0f), m_up(0.0f, 1.0f, 0.0f),
      m_right(0), m_worldUp(0.0f, 1.0f, 0.0f), m_yaw(-90.0f), m_pitch(0.0f), m_baseSpeed(1.0f), m_speed(1.0f),
      m_boostMultiplier(3.0f), m_sensitivity(30.0f), m_mousePressed(false) {
  reset();
}

void Camera::update() {
  handleKeyInput();
  m_speed = SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LSHIFT] ? m_baseSpeed * m_boostMultiplier : m_baseSpeed;

  updateCursorCapture();
}

void Camera::processEvent(const SDL_Event *event) {
  if (event->type == SDL_EVENT_MOUSE_MOTION && m_mousePressed) {
    const auto deltaTIme = g_container.m_time->deltaTime();
    const float xOffset = event->motion.xrel * m_sensitivity * deltaTIme;
    const float yOffset = -event->motion.yrel * m_sensitivity * deltaTIme;

    m_yaw += xOffset;
    m_pitch += yOffset;

    if (m_pitch > 89.0f)
      m_pitch = 89.0f;
    if (m_pitch < -89.0f)
      m_pitch = -89.0f;

    updateCameraVectors();
  }

  if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_RIGHT) {
    m_mousePressed = true;
  }

  if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_RIGHT) {
    m_mousePressed = false;
  }
}

void Camera::setActive(const bool active) {
  m_mousePressed = false;
  updateCursorCapture();
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::vec3 Camera::getPosition() const {
  return m_position;
}

void Camera::reset() {
  m_position = m_initialPosition;
  lookAtOrigin();
  updateCameraVectors();
}

void Camera::lookAtOrigin() {
  if (m_position != glm::vec3(0.0f)) {
    const glm::vec3 dirToOrigin = glm::normalize(-m_position);
    m_yaw = static_cast<float>(glm::degrees(atan2(dirToOrigin.z, dirToOrigin.x)));
    m_pitch = static_cast<float>(glm::degrees(asin(dirToOrigin.y)));
  }
}

void Camera::updateCameraVectors() {
  glm::vec3 front;
  front.x = static_cast<float>(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
  front.y = static_cast<float>(sin(glm::radians(m_pitch)));
  front.z = static_cast<float>(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
  m_front = glm::normalize(front);

  m_right = glm::normalize(glm::cross(m_front, m_worldUp));
  m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::handleKeyInput() {
  const bool *keys = SDL_GetKeyboardState(nullptr);
  const float frameSpeed = m_speed * g_container.m_time->deltaTime();

  if (keys[SDL_SCANCODE_W]) {
    m_position += m_front * frameSpeed;
  }

  if (keys[SDL_SCANCODE_S]) {
    m_position -= m_front * frameSpeed;
  }

  if (keys[SDL_SCANCODE_A]) {
    m_position -= m_right * frameSpeed;
  }

  if (keys[SDL_SCANCODE_D]) {
    m_position += m_right * frameSpeed;
  }

  if (keys[SDL_SCANCODE_SPACE]) {
    m_position += m_up * frameSpeed;
  }

  if (keys[SDL_SCANCODE_Q]) {
    m_position -= m_up * frameSpeed;
  }
}

void Camera::updateCursorCapture() const {
  if (m_mousePressed) {
    SDL_SetWindowRelativeMouseMode(SDL_GetMouseFocus(), true);
  } else {
    SDL_SetWindowRelativeMouseMode(SDL_GetMouseFocus(), false);
  }
}

} // namespace App
