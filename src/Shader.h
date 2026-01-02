#pragma once

#include <string>

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
  unsigned int ID;

  Shader(const std::string &name);

  Shader(const std::string &vertexPath, const std::string &fragmentPath);

  ~Shader();

  void use() const {
    glUseProgram(ID);
  }

  void set(const std::string &name, const bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
  }

  void set(const std::string &name, const int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }

  void set(const std::string &name, const float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }

  void set(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }

  void set(const std::string &name, const float x, const float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
  }

  void set(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }

  void set(const std::string &name, const float x, const float y, const float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  }

  void set(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }

  void set(const std::string &name, const float x, const float y, const float z, const float w) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
  }

  void set(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
  }

  void set(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
  }

  void set(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
  }

private:
  static void checkCompileErrors(GLuint shader, ShaderType type);

  static uint compile(const std::string &code, ShaderType type);
};

} // namespace App
