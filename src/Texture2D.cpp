#include "Texture2D.h"

#include <utility>
#include <filesystem>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <stb_image.h>

Texture2D::Texture2D(std::string path) : m_id(0), m_path(std::move(path)), m_isLoaded(false) {
}

Texture2D::~Texture2D() {
  free();
}

void Texture2D::load() {
  if (m_isLoaded) {
    SPDLOG_WARN("Trying to reload texture: {}", m_path);
    free();
  }

  if (!std::filesystem::exists(m_path)) {
    SPDLOG_WARN("Texture not found: {}", m_path);
  }

  int width, height, channels;
  unsigned char *data = stbi_load(m_path.c_str(), &width, &height, &channels, 0);

  if (!data) {
    SPDLOG_ERROR("Failed to load texture: {}", m_path);
    return;
  }

  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLint format = GL_RGB;

  if (channels == 1)
    format = GL_RED;
  else if (channels == 3)
    format = GL_RGB;
  else if (channels == 4)
    format = GL_RGBA;

  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  constexpr auto logMessage = "Texture loaded: {} ({}x{}, {} channels) - ID: {}";

  SPDLOG_DEBUG(logMessage, m_path, width, height, channels, m_id);
}

unsigned int Texture2D::getId() const {
  return m_id;
}

const std::string &Texture2D::getPath() const {
  return m_path;
}

bool Texture2D::isLoaded() const {
  return m_isLoaded;
}

void Texture2D::free() const {
  if (m_id && m_isLoaded) {
    glad_glDeleteTextures(1, &m_id);
  }
}
