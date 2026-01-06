#pragma once

#include <memory>

#include "Shader.h"
#include "Texture2D.h"

class Material {
public:
  std::shared_ptr<App::Shader> getShader() {
    return shader;
  }

  void bindTextures() const;
  void applyUniforms() const;

  void setFloat(const std::string &name, const float value) {
    floatUniforms[name] = value;
  }
  void setVec3(const std::string &name, const glm::vec3 &value) {
    vec3Uniforms[name] = value;
  }

private:
  std::shared_ptr<App::Shader> shader;
  std::shared_ptr<Texture2D> diffuse;
  std::shared_ptr<Texture2D> specular;
  std::shared_ptr<Texture2D> normal;
  std::unordered_map<std::string, float> floatUniforms;
  std::unordered_map<std::string, glm::vec3> vec3Uniforms;
};
