#include "Material.h"

void Material::bindTextures() {
  if (diffuse) {
    glActiveTexture(GL_TEXTURE0 + DIFFUSE_TEXTURE_INDEX);
    setIntUniform(DIFFUSE_TEXTURE_UNIFORM_NAME, DIFFUSE_TEXTURE_INDEX);
    diffuse->bind();
  }

  // Texture unit 1: Specular
  if (specular) {
    glActiveTexture(GL_TEXTURE0 + SPECULAR_TEXTURE_INDEX);
    setIntUniform(SPECULAR_TEXTURE_UNIFORM_NAME, SPECULAR_TEXTURE_INDEX);
    specular->bind();
  }

  // Texture unit 2: Normal
  if (normal) {
    glActiveTexture(GL_TEXTURE0 + NORMAL_TEXTURE_INDEX);
    setIntUniform(NORMAL_TEXTURE_UNIFORM_NAME, NORMAL_TEXTURE_INDEX);
    normal->bind();
  }
}

void Material::applyUniforms() const {
  for (auto const &[name, val] : floatUniforms) {
    shader->set(name, val);
  }

  for (auto const &[name, val] : vec3Uniforms) {
    shader->set(name, val);
  }

  for (auto const &[name, val] : intUniforms) {
    shader->set(name, val);
  }
}
