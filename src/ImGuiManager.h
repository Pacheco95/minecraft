#pragma once

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

namespace App {
class ImGuiManager {
public:
  ImGuiManager() = default;

  ~ImGuiManager();

  void setup();

  void newFrame();

  void populateFrame();

  void renderFrame();

  void processEvent(const SDL_Event *event);

  const ImGuiIO &io() const;
};
} // namespace App
