#pragma once
#include "Window.h"

namespace App {

class Engine {
public:
  explicit Engine(Window *gameWindow);

  [[nodiscard]] Window *getWindow() const;

private:
  Window *m_gameWindow;
};

} // namespace App
