#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform final {
public:
  Transform();
  ~Transform() = default;

  // --- Hierarchy Management ---
  void SetParent(Transform *parent, bool keepWorldTransform = true);
  Transform *GetParent() const {
    return m_parent;
  }
  const std::vector<Transform *> &GetChildren() const {
    return m_children;
  }

  // --- Position ---
  void SetLocalPosition(const glm::vec3 &pos);
  glm::vec3 GetLocalPosition() const {
    return m_localPosition;
  }

  void SetPosition(const glm::vec3 &pos); // World Position
  glm::vec3 GetPosition() const;          // World Position

  // --- Rotation ---
  void SetLocalRotation(const glm::quat &rot);
  glm::quat GetLocalRotation() const {
    return m_localRotation;
  }

  void SetRotation(const glm::quat &rot); // World Rotation
  glm::quat GetRotation() const;          // World Rotation

  // Euler Angles helpers (Degrees)
  void SetLocalEulerAngles(const glm::vec3 &eulerAngles);
  glm::vec3 GetLocalEulerAngles() const;

  // --- Scale ---
  void SetLocalScale(const glm::vec3 &scale);
  glm::vec3 GetLocalScale() const {
    return m_localScale;
  }

  // Note: World scale is complex due to skewing, Unity calls this "lossyScale"
  // We will provide a standard global matrix extraction for this.

  // --- Vectors (Direction) ---
  glm::vec3 GetForward() const;
  glm::vec3 GetUp() const;
  glm::vec3 GetRight() const;

  // --- Matrix Retrieval ---
  // This is what you pass to your Shader (Uniform "model")
  const glm::mat4 &GetModelMatrix() const;

  // --- Utility Operations ---
  void Translate(const glm::vec3 &translation);
  void Rotate(const glm::vec3 &axis, float angle);
  void Rotate(const glm::quat &rotation);
  void LookAt(const glm::vec3 &target, const glm::vec3 &worldUp = glm::vec3(0, 1, 0));

protected:
  // Internal method to mark this and all children as dirty
  void SetDirty();

private:
  // Local Space Data
  glm::vec3 m_localPosition;
  glm::quat m_localRotation;
  glm::vec3 m_localScale;

  // Hierarchy
  Transform *m_parent;
  std::vector<Transform *> m_children;

  // Cached Model Matrix (Optimization)
  mutable glm::mat4 m_modelMatrix;
  mutable bool m_isDirty;

  // Helper to update the matrix if dirty
  glm::mat4 GetLocalModelMatrix() const;
};
