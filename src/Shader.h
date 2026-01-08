#pragma once

#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

namespace App {
enum class ShaderType {
  VERTEX,
  FRAGMENT,
  PROGRAM
};

constexpr const char *const shaderTypes[] = {"VERTEX", "FRAGMENT", "PROGRAM"};

class Shader {
public:
  explicit Shader(const std::string &name);
  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  Shader(const char *vertexName, const char *fragmentName);

  ~Shader();

  void use() const {
    glUseProgram(m_id);
  }

  void set(const std::string &name, const bool value) {
    glUniform1i(getUniformLocation(name), static_cast<int>(value));
  }

  void set(const std::string &name, const std::size_t value) {
    glUniform1ui(getUniformLocation(name), value);
  }

  void set(const std::string &name, const int value) {
    glUniform1i(getUniformLocation(name), value);
  }

  void set(const std::string &name, const float value) {
    glUniform1f(getUniformLocation(name), value);
  }

  void set(const std::string &name, const glm::vec2 &value) {
    glUniform2fv(getUniformLocation(name), 1, &value[0]);
  }

  void set(const std::string &name, const float x, const float y) {
    glUniform2f(getUniformLocation(name), x, y);
  }

  void set(const std::string &name, const glm::vec3 &value) {
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
  }

  void set(const std::string &name, const float x, const float y, const float z) {
    glUniform3f(getUniformLocation(name), x, y, z);
  }

  void set(const std::string &name, const glm::vec4 &value) {
    glUniform4fv(getUniformLocation(name), 1, &value[0]);
  }

  void set(const std::string &name, const float x, const float y, const float z, const float w) {
    glUniform4f(getUniformLocation(name), x, y, z, w);
  }

  void set(const std::string &name, const glm::mat2 &mat) {
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
  }

  void set(const std::string &name, const glm::mat3 &mat) {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
  }

  void set(const std::string &name, const glm::mat4 &mat) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
  }

private:
  unsigned int m_id;

  std::string m_vertexPath;
  std::string m_fragmentPath;
  std::unordered_map<std::string, GLint> m_uniformLocations{};

  GLint getUniformLocation(const std::string &name);
  static void checkCompileErrors(GLuint shader, ShaderType type);
  static uint compile(const std::string &code, ShaderType type);
};

} // namespace App
