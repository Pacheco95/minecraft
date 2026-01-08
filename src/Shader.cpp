#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "Config.h"

constexpr auto SHADER_PATH = "resources/shaders/";

namespace App {

std::string loadShaderFile(const char *path) {
  std::string content;
  std::ifstream file;

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    file.open(path);
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    content = stream.str();
  } catch (std::ifstream::failure &e) {
    SPDLOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {} {}", path, e.what());
  }

  return content;
}

Shader::Shader(const std::string &name) : Shader(SHADER_PATH + name + ".vert", SHADER_PATH + name + ".frag") {
}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
    : m_vertexPath(vertexPath), m_fragmentPath(fragmentPath) {
  const std::string vertexCode = loadShaderFile(vertexPath.c_str());
  const std::string fragmentCode = loadShaderFile(fragmentPath.c_str());

  const uint vertex = compile(vertexCode, ShaderType::VERTEX);
  const uint fragment = compile(fragmentCode, ShaderType::FRAGMENT);

  // shader Program
  m_id = glCreateProgram();
  glAttachShader(m_id, vertex);
  glAttachShader(m_id, fragment);
  glLinkProgram(m_id);

  checkCompileErrors(m_id, ShaderType::PROGRAM);

  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::Shader(const char *const vertexName, const char *const fragmentName)
    : Shader(std::string(SHADER_PATH) + vertexName, std::string(SHADER_PATH) + fragmentName) {
}

Shader::~Shader() {
  glDeleteProgram(m_id);
}

void Shader::checkCompileErrors(const GLuint shader, const ShaderType type) {
  GLint success;
  constexpr uint bufferSize = 1024;
  static GLchar errorMessage[bufferSize];

  const auto index = static_cast<uint>(type);
  const auto shaderTypeStr = shaderTypes[index];

  if (type != ShaderType::PROGRAM) {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, bufferSize, nullptr, errorMessage);
      SPDLOG_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {}\n\t{}", shaderTypeStr, errorMessage);
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, bufferSize, nullptr, errorMessage);
      SPDLOG_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {}\n\t{}", shaderTypeStr, errorMessage);
    }
  }
}

uint Shader::compile(const std::string &code, const ShaderType type) {
  const uint shaderId = glCreateShader(type == ShaderType::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
  const auto shaderCodePtr = code.c_str();
  glShaderSource(shaderId, 1, &shaderCodePtr, nullptr);
  glCompileShader(shaderId);
  checkCompileErrors(shaderId, type);
  return shaderId;
}

GLint Shader::getUniformLocation(const std::string &name) {
  if (const auto locationPtr = m_uniformLocations.find(name); locationPtr != m_uniformLocations.end()) {
    return locationPtr->second;
  }

  const GLint location = glGetUniformLocation(m_id, name.c_str());

  if (Config::Core::DEBUG_MODE) {
    if (location == -1) {
      if (m_vertexPath.contains("skeleton") && m_fragmentPath.contains("skeleton")) {
        SPDLOG_WARN("Trying to set unknown uniform: {}", name);
      }
    }
  }

  m_uniformLocations.insert({name, location});
  return location;
}

} // namespace App
