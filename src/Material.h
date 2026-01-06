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

private:
  std::shared_ptr<App::Shader> shader;
  std::shared_ptr<Texture2D> diffuse;
  std::shared_ptr<Texture2D> specular;
  std::shared_ptr<Texture2D> normal;
};
