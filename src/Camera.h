#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL3/SDL.h>

namespace App {

class Camera {
public:
  explicit Camera(const glm::vec3 &position = glm::vec3(0));

  void update();
  void processEvent(const SDL_Event *event);

  void setActive(bool active);
  [[nodiscard]] bool isActive() const;

  [[nodiscard]] glm::mat4 getViewMatrix() const;
  [[nodiscard]] glm::vec3 getPosition() const;

private:
  glm::vec3 m_position;
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

  bool m_active;
  bool m_mousePressed;

  void updateCameraVectors();
  void handleKeyInput();
  void updateCursorCapture() const;
};

} // namespace App
