#pragma once

#include <glm/glm.hpp>
#include <SDL3/SDL.h>

namespace App {

class Camera {
public:
  explicit Camera(const glm::vec3 &position = glm::vec3(0));

  void update();
  void processEvent(const SDL_Event *event);

  void setActive(bool active);

  [[nodiscard]] glm::mat4 getViewMatrix() const;
  [[nodiscard]] const glm::vec3 &getPosition() const;
  void reset();

private:
  glm::vec3 m_position;
  const glm::vec3 m_initialPosition;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_worldUp;

  float m_yaw;
  float m_pitch;
  float m_baseSpeed;
  float m_speed;
  float m_boostMultiplier;
  float m_sensitivity;

  bool m_mousePressed;

  void lookAtOrigin();
  void updateCameraVectors();
  void handleKeyInput();
  void updateCursorCapture() const;
};
} // namespace App
