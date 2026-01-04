#pragma once


#include <memory>

#include "ImGuiManager.h"
#include "ShaderCache.h"
#include "Time.h"
#include "Window.h"
#include "Camera.h"
#include "FloorGrid.h"

namespace App {
class Container {
public:
  std::shared_ptr<Window> m_window = nullptr;
  std::shared_ptr<ImGuiManager> m_imguiManager = nullptr;
  std::shared_ptr<Time> m_time = nullptr;
  std::shared_ptr<ShaderCache> m_shaderCache = nullptr;
  std::shared_ptr<Camera> m_camera = nullptr;
  std::shared_ptr<FloorGrid> m_floorGrid = nullptr;

  Container(const Container &) = delete;
  Container &operator=(const Container &) = delete;

  Container(Container &&) = delete;
  Container &operator=(Container &&) = delete;

  static Container &getInstance() {
    static Container instance;
    return instance;
  }

  void init() {
    m_window = std::make_shared<Window>();
    m_imguiManager = std::make_shared<ImGuiManager>();
    m_time = std::make_shared<Time>();
    m_shaderCache = std::make_shared<ShaderCache>();
    m_camera = std::make_shared<Camera>(glm::vec3(0.0f, 3.0f, 0.0f));
    m_floorGrid = std::make_shared<FloorGrid>();
  }

  void dispose() {
    if (m_window) {
      m_window->dispose();
      m_window = nullptr;
    }
  }

private:
  Container() = default;
  ~Container() = default;
};

} // namespace App

#define g_container (Container::getInstance())

inline const App::Container &container = App::Container::getInstance();

// Since the Container object lives for the entire program lifetime it is safe to dereference the pointers
#define g_window (*container.m_window)
#define g_imguiManager (*container.m_imguiManager)
#define g_time (*container.m_time)
#define g_shaderCache (*container.m_shaderCache)
#define g_camera (*container.m_camera)
#define g_floorGrid (*container.m_floorGrid)
