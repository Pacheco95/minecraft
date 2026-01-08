#include "Transform.h"

Transform::Transform()
    : m_localPosition(0.0f), m_localRotation(glm::vec3(0.0f)), m_localScale(1.0f), m_parent(nullptr),
      m_modelMatrix(1.0f), m_isDirty(true) {
}

void Transform::SetDirty() {
  // If we are already dirty, no need to recurse,
  // BUT children might assume their parent is clean if we don't signal.
  // Simple approach: Always flag self and recurse down.
  if (!m_isDirty) {
    m_isDirty = true;
    for (Transform *child : m_children) {
      child->SetDirty();
    }
  }
}

glm::mat4 Transform::GetLocalModelMatrix() const {
  const glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_localPosition);
  const glm::mat4 rotation = glm::mat4_cast(m_localRotation);
  const glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_localScale);

  return translation * rotation * scale;
}

const glm::mat4 &Transform::GetModelMatrix() const {
  if (m_isDirty) {
    const glm::mat4 localMatrix = GetLocalModelMatrix();

    if (m_parent) {
      m_modelMatrix = m_parent->GetModelMatrix() * localMatrix;
    } else {
      m_modelMatrix = localMatrix;
    }
    m_isDirty = false;
  }

  return m_modelMatrix;
}

// --- Hierarchy ---

void Transform::SetParent(Transform *parent, const bool keepWorldTransform) {
  if (m_parent == parent)
    return;

  // Logic to maintain world position despite new parent
  if (keepWorldTransform) {
    const glm::vec3 worldPos = GetPosition();
    const glm::quat worldRot = GetRotation();
    // Scale is tricky, usually ignored or approximations used in simple engines

    // Remove self from old parent
    if (m_parent) {
      auto &siblings = m_parent->m_children;
      std::erase(siblings, this);
    }

    m_parent = parent;

    if (m_parent) {
      m_parent->m_children.push_back(this);

      // Convert World to New Local
      // NewLocal = Inv(ParentWorld) * World
      glm::mat4 parentModelInv = glm::inverse(m_parent->GetModelMatrix());
      glm::vec3 newLocalPos = glm::vec3(parentModelInv * glm::vec4(worldPos, 1.0f));

      SetLocalPosition(newLocalPos);
      // Rotation is slightly more complex, simplified here:
      SetRotation(worldRot);
    } else {
      // Unparenting (Becoming root)
      SetLocalPosition(worldPos);
      SetRotation(worldRot);
    }
  } else {
    // Just swap parent, keeping local transforms (object will jump)
    if (m_parent) {
      auto &siblings = m_parent->m_children;
      std::erase(siblings, this);
    }
    m_parent = parent;
    if (m_parent)
      m_parent->m_children.push_back(this);
  }
  SetDirty();
}

// --- Position ---

void Transform::SetLocalPosition(const glm::vec3 &pos) {
  m_localPosition = pos;
  SetDirty();
}

void Transform::SetPosition(const glm::vec3 &pos) {
  if (m_parent) {
    // Calculate local position relative to parent
    // Local = Inverse(ParentMatrix) * WorldPos
    glm::mat4 parentMatrix = m_parent->GetModelMatrix();
    glm::mat4 inverseParent = glm::inverse(parentMatrix);
    m_localPosition = glm::vec3(inverseParent * glm::vec4(pos, 1.0f));
  } else {
    m_localPosition = pos;
  }
  SetDirty();
}

glm::vec3 Transform::GetPosition() const {
  return GetModelMatrix()[3];
}

// --- Rotation ---

void Transform::SetLocalRotation(const glm::quat &rot) {
  m_localRotation = rot;
  SetDirty();
}

void Transform::SetRotation(const glm::quat &rot) {
  if (m_parent) {
    // WorldRot = ParentRot * LocalRot
    // LocalRot = Inverse(ParentRot) * WorldRot
    const glm::quat parentRot = m_parent->GetRotation();
    m_localRotation = glm::inverse(parentRot) * rot;
  } else {
    m_localRotation = rot;
  }
  SetDirty();
}

glm::quat Transform::GetRotation() const {
  if (m_parent) {
    return m_parent->GetRotation() * m_localRotation;
  }
  return m_localRotation;
}

void Transform::SetLocalEulerAngles(const glm::vec3 &eulerAngles) {
  m_localRotation = glm::quat(glm::radians(eulerAngles));
  SetDirty();
}

glm::vec3 Transform::GetLocalEulerAngles() const {
  return glm::degrees(glm::eulerAngles(m_localRotation));
}

// --- Scale ---

void Transform::SetLocalScale(const glm::vec3 &scale) {
  m_localScale = scale;
  SetDirty();
}

// --- Direction Vectors ---

glm::vec3 Transform::GetForward() const {
  return GetRotation() * glm::vec3(0, 0, 1); // Assuming Z is forward
}

glm::vec3 Transform::GetUp() const {
  return GetRotation() * glm::vec3(0, 1, 0);
}

glm::vec3 Transform::GetRight() const {
  return GetRotation() * glm::vec3(1, 0, 0);
}

// --- Operations ---

void Transform::Translate(const glm::vec3 &translation) {
  m_localPosition += translation;
  SetDirty();
}

void Transform::Rotate(const glm::vec3 &axis, const float angle) {
  m_localRotation = glm::rotate(m_localRotation, glm::radians(angle), axis);
  SetDirty();
}

void Transform::Rotate(const glm::quat &rotation) {
  m_localRotation = rotation * m_localRotation;
  SetDirty();
}

void Transform::LookAt(const glm::vec3 &target, const glm::vec3 &worldUp) {
  glm::vec3 dir = glm::normalize(target - GetPosition());
  // Use GLM's quaternion LookAt
  // Note: You might need to conjugate depending on your coordinate system (LHS vs RHS)
  glm::quat lookRot = glm::quatLookAt(dir, worldUp);
  SetRotation(lookRot);
}
