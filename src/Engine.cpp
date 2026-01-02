#include "Engine.h"

namespace App {
Engine::Engine(Window *gameWindow) : m_gameWindow(gameWindow) {
}
} // namespace App

App::Window *App::Engine::getWindow() const {
  return m_gameWindow;
}

void App::Engine::processFrame(double deltaTime) const {
}
