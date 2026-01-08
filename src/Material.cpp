#include "Material.h"

void Material::bindTextures() {
  if (m_diffuseTexture) {
    glActiveTexture(GL_TEXTURE0 + DIFFUSE_TEXTURE_INDEX);
    setIntUniform(DIFFUSE_TEXTURE_UNIFORM_NAME, DIFFUSE_TEXTURE_INDEX);
    m_diffuseTexture->bind();
  }

  // Texture unit 1: Specular
  if (m_specularTexture) {
    glActiveTexture(GL_TEXTURE0 + SPECULAR_TEXTURE_INDEX);
    setIntUniform(SPECULAR_TEXTURE_UNIFORM_NAME, SPECULAR_TEXTURE_INDEX);
    m_specularTexture->bind();
  }

  // Texture unit 2: Normal
  if (m_normalTexture) {
    glActiveTexture(GL_TEXTURE0 + NORMAL_TEXTURE_INDEX);
    setIntUniform(NORMAL_TEXTURE_UNIFORM_NAME, NORMAL_TEXTURE_INDEX);
    m_normalTexture->bind();
  }
}

void Material::applyUniforms() const {
  for (auto const &[name, val] : m_floatUniforms) {
    m_shader->set(name, val);
  }

  for (auto const &[name, val] : m_vec3Uniforms) {
    m_shader->set(name, val);
  }

  for (auto const &[name, val] : m_intUniforms) {
    m_shader->set(name, val);
  }
}
