#include "Material.h"

void Material::bindTextures() const {
  if (diffuse) {
    glActiveTexture(GL_TEXTURE0);
    diffuse->bind();
  }

  // Texture unit 1: Specular
  if (specular) {
    glActiveTexture(GL_TEXTURE1);
    specular->bind();
  }

  // Texture unit 2: Normal
  if (normal) {
    glActiveTexture(GL_TEXTURE2);
    normal->bind();
  }
}
