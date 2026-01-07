#pragma once

#include <memory>

#include "Shader.h"
#include "Texture2D.h"

// Vec4 uniforms
constexpr auto DIFFUSE_COLOR_UNIFORM_NAME = "uDiffuseColor";
constexpr auto AMBIENT_COLOR_UNIFORM_NAME = "uAmbientColor";
constexpr auto SPECULAR_COLOR_UNIFORM_NAME = "uSpecularColor";
constexpr auto EMISSIVE_COLOR_UNIFORM_NAME = "uEmissiveColor";
constexpr auto TRANSPARENT_COLOR_UNIFORM_NAME = "uTransparentColor";
constexpr auto REFLECTIVE_COLOR_UNIFORM_NAME = "uReflectiveColor";

// Float uniforms
/// A value from 0.0 (transparent) to 1.0 (opaque).
constexpr auto OPACITY_UNIFORM_NAME = "uOpacity";

/// The specular exponent (how "tight" the highlight is).
constexpr auto SHININESS_UNIFORM_NAME = "uShininess";

/// A multiplier for the specular color.
constexpr auto SHININESS_STRENGTH_UNIFORM_NAME = "uShininessStrength";

/// How much of the reflection color to mix in.
constexpr auto REFLECTIVITY_UNIFORM_NAME = "uReflectivity";

/// The Index of Refraction (IoR), usually 1.0 or higher (e.g., 1.33 for water).
constexpr auto REFRACTION_INDEX_UNIFORM_NAME = "uRefractionIndex";

/// A multiplier for the intensity of the normal/bump map.
constexpr auto BUMP_SCALING_UNIFORM_NAME = "uBumpScaling";

/// Similar to opacity, used by some formats like FBX.
constexpr auto TRANSPARENCY_FACTOR_UNIFORM_NAME = "uTransparencyFactor";

class Material {
public:
  std::shared_ptr<App::Shader> getShader() {
    return shader;
  }

  void bindTextures() const;
  void applyUniforms() const;

  void setUniform(const std::string &name, const float value) {
    floatUniforms[name] = value;
  }

  void setUniform(const std::string &name, const glm::vec3 &value) {
    vec3Uniforms[name] = value;
  }

  void setUniform(const std::string &name, const glm::vec4 &value) {
    vec4Uniforms[name] = value;
  }

  void setDiffuseTex(const std::shared_ptr<Texture2D> &diffuseTexture) {
    diffuse = diffuseTexture;
  }

  void setSpecularTex(const std::shared_ptr<Texture2D> &specularTexture) {
    specular = specularTexture;
  }

  void setNormalTex(const std::shared_ptr<Texture2D> &normalTexture) {
    normal = normalTexture;
  }

private:
  std::shared_ptr<App::Shader> shader;
  std::shared_ptr<Texture2D> diffuse;
  std::shared_ptr<Texture2D> specular;
  std::shared_ptr<Texture2D> normal;
  std::unordered_map<std::string, float> floatUniforms;
  std::unordered_map<std::string, glm::vec3> vec3Uniforms;
  std::unordered_map<std::string, glm::vec4> vec4Uniforms;
};
