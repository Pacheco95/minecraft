#pragma once

#include <memory>

#include "Shader.h"
#include "Texture.h"

enum TextureIndex {
  DIFFUSE_TEXTURE_INDEX,
  SPECULAR_TEXTURE_INDEX,
  NORMAL_TEXTURE_INDEX,
};

// Vec4 uniforms
constexpr auto DIFFUSE_COLOR_UNIFORM_NAME = "uMaterial.diffuseColor";
constexpr auto AMBIENT_COLOR_UNIFORM_NAME = "uMaterial.ambientColor";
constexpr auto SPECULAR_COLOR_UNIFORM_NAME = "uMaterial.specularColor";
constexpr auto EMISSIVE_COLOR_UNIFORM_NAME = "uMaterial.emissiveColor";
constexpr auto TRANSPARENT_COLOR_UNIFORM_NAME = "uMaterial.transparentColor";
constexpr auto REFLECTIVE_COLOR_UNIFORM_NAME = "uMaterial.reflectiveColor";

// Sampler uniforms
constexpr auto DIFFUSE_TEXTURE_UNIFORM_NAME = "uMaterial.diffuseTexture";
constexpr auto SPECULAR_TEXTURE_UNIFORM_NAME = "uMaterial.specularTexture";
constexpr auto NORMAL_TEXTURE_UNIFORM_NAME = "uMaterial.normalTexture";

// Float uniforms
/// A value from 0.0 (transparent) to 1.0 (opaque).
constexpr auto OPACITY_UNIFORM_NAME = "uMaterial.opacity";

/// The specular exponent (how "tight" the highlight is).
constexpr auto SHININESS_UNIFORM_NAME = "uMaterial.shininess";

/// A multiplier for the specular color.
constexpr auto SHININESS_STRENGTH_UNIFORM_NAME = "uMaterial.shininessStrength";

/// How much of the reflection color to mix in.
constexpr auto REFLECTIVITY_UNIFORM_NAME = "uMaterial.reflectivity";

/// The Index of Refraction (IoR), usually 1.0 or higher (e.g., 1.33 for water).
constexpr auto REFRACTION_INDEX_UNIFORM_NAME = "uMaterial.refractionIndex";

/// A multiplier for the intensity of the normal/bump map.
constexpr auto BUMP_SCALING_UNIFORM_NAME = "uMaterial.bumpScaling";

/// Similar to opacity, used by some formats like FBX.
constexpr auto TRANSPARENCY_FACTOR_UNIFORM_NAME = "uMaterial.transparencyFactor";

class Material {
public:
  std::shared_ptr<App::Shader> getShader() {
    return m_shader;
  }

  void bindTextures();
  void applyUniforms() const;

  void setShader(const std::shared_ptr<App::Shader> &shader) {
    m_shader = shader;
  }

  void setUniform(const std::string &name, const float value) {
    m_floatUniforms[name] = value;
  }

  void setIntUniform(const std::string &name, const int value) {
    m_intUniforms[name] = value;
  }

  void setUniform(const std::string &name, const glm::vec3 &value) {
    m_vec3Uniforms[name] = value;
  }

  void setUniform(const std::string &name, const glm::vec4 &value) {
    m_vec4Uniforms[name] = value;
  }

  void setDiffuseTex(const std::shared_ptr<Texture> &diffuseTexture) {
    m_diffuseTexture = diffuseTexture;
  }

  void setSpecularTex(const std::shared_ptr<Texture> &specularTexture) {
    m_specularTexture = specularTexture;
  }

  void setNormalTex(const std::shared_ptr<Texture> &normalTexture) {
    m_normalTexture = normalTexture;
  }

private:
  std::shared_ptr<App::Shader> m_shader;
  std::shared_ptr<Texture> m_diffuseTexture;
  std::shared_ptr<Texture> m_specularTexture;
  std::shared_ptr<Texture> m_normalTexture;
  std::unordered_map<std::string, float> m_floatUniforms;
  std::unordered_map<std::string, glm::vec3> m_vec3Uniforms;
  std::unordered_map<std::string, glm::vec4> m_vec4Uniforms;
  std::unordered_map<std::string, int> m_intUniforms;
};
