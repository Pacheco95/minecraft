#pragma once

namespace App::Config {
namespace Window {
constexpr auto TITLE = "Minecraft";
constexpr int WIDTH = 1024;
constexpr int HEIGHT = 1024;
inline float CLEAR_COLOR[] = {0.1, 0.1, 0.1};
} // namespace Window

namespace Core {
constexpr int OPENGL_VERSION_MAJOR = 3;
constexpr int OPENGL_VERSION_MINOR = 3;
constexpr auto GLSL_VERSION = "#version 330";
} // namespace Core
} // namespace App::Config
