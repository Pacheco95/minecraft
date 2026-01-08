#pragma once

namespace App::Config {
namespace Window {
constexpr auto TITLE = "Minecraft";
constexpr int WIDTH = 1024;
constexpr int HEIGHT = 1024;
inline float CLEAR_COLOR[] = {0.0f, 0.0f, 0.0f};
} // namespace Window

namespace Core {
constexpr int OPENGL_VERSION_MAJOR = 3;
constexpr int OPENGL_VERSION_MINOR = 3;
constexpr auto GLSL_VERSION = "#version 330";
constexpr auto DEBUG_MODE = false;
} // namespace Core

namespace Renderer {
constexpr auto DEFAULT_VERTEX_SHADER = "skeleton.vert";
constexpr auto DEFAULT_FRAGMENT_SHADER = "skeleton.frag";
constexpr auto COLOR_PLACEHOLDER = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
} // namespace Renderer
} // namespace App::Config
