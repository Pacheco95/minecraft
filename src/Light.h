#pragma once

#include <glm/glm.hpp>

enum class LightType : uint32_t {
  Directional,
  Point,
  Spot,
};

struct Light {
  LightType type;

  glm::vec4 color = glm::vec4(1.0f);
  float intensity = 1.0f;

  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

  float constant = 1.0f;
  float linear = 0.09f;
  float quadratic = 0.032f;

  float innerCutoff = glm::cos(glm::radians(12.5f));
  float outerCutoff = glm::cos(glm::radians(17.5f));

  [[nodiscard]] constexpr auto typeStr() const {
    switch (type) {
    case LightType::Directional:
      return "Directional";
    case LightType::Point:
      return "Point";
    case LightType::Spot:
      return "Spot";
    }

    throw std::runtime_error("Unknown LightType");
  }

  static Light Directional(const glm::vec3 &direction, const glm::vec4 &color = glm::vec4(1.0f),
                           const float intensity = 1.0f) {
    return {
        .type = LightType::Directional,
        .color = color,
        .intensity = intensity,
        .direction = glm::normalize(direction),
    };
  }

  static Light Point(const glm::vec3 &position, const glm::vec4 &color = glm::vec4(1.0f), const float intensity = 1.0f,
                     const float constant = 1.0f, const float linear = 0.09f, const float quadratic = 0.032f) {
    return {
        .type = LightType::Point,
        .color = color,
        .intensity = intensity,
        .position = position,
        .constant = constant,
        .linear = linear,
        .quadratic = quadratic,
    };
  }

  static Light Spot(const glm::vec3 &position, const glm::vec3 &direction, const float innerAngleDeg,
                    const float outerAngleDeg, const auto &color = glm::vec4(1.0f), const float intensity = 1.0f) {

    return {
        .type = LightType::Spot,
        .color = color,
        .intensity = intensity,
        .position = position,
        .direction = glm::normalize(direction),
        .innerCutoff = glm::cos(glm::radians(innerAngleDeg)),
        .outerCutoff = glm::cos(glm::radians(outerAngleDeg)),
    };
  }
};
