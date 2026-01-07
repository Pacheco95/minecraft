#pragma once

#include <memory>

#include "Shader.h"
#include "Texture2D.h"

constexpr auto DIFFUSE_COLOR_UNIFORM_NAME = "uDiffuseColor";
constexpr auto AMBIENT_COLOR_UNIFORM_NAME = "uAmbientColor";
constexpr auto SPECULAR_COLOR_UNIFORM_NAME = "uSpecularColor";
constexpr auto EMISSIVE_COLOR_UNIFORM_NAME = "uEmissiveColor";
constexpr auto TRANSPARENT_COLOR_UNIFORM_NAME = "uTransparentColor";
constexpr auto REFLECTIVE_COLOR_UNIFORM_NAME = "uReflectiveColor";

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
