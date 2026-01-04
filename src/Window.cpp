#include "Window.h"

#include <memory>

#include <imgui.h>

#include "Container.h"
#include "Model.h"
#include "Config.h"

namespace App {

Model model3d;

SDL_AppResult Window::setup() {
  SDL_SetAppMetadata("Minecraft", "0.1.0", "com.example.minecraft");

  if (!SDL_Init(SDL_INIT_VIDEO) != 0) {
    SPDLOG_CRITICAL("Couldn't initialize SDL: {}", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Configure OpenGL attributes before creating the window/context
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  const float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

  constexpr SDL_WindowFlags windowFlags =
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;

  const int width = static_cast<int>(Config::Window::WIDTH * mainScale);
  const int height = static_cast<int>(Config::Window::HEIGHT * mainScale);

  if (!SDL_CreateWindowAndRenderer(Config::Window::TITLE, width, height, windowFlags, &m_sdlWindow, &m_sdlRenderer)) {
    SPDLOG_CRITICAL("Couldn't create window/renderer: {}", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetWindowPosition(m_sdlWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  m_glContext = SDL_GL_CreateContext(m_sdlWindow);

  if (!m_glContext) {
    SPDLOG_CRITICAL("Couldn't create OpenGL context: {}", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  const auto openGlProcedureLoader = reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress);

  if (!gladLoadGLLoader(openGlProcedureLoader)) {
    SPDLOG_CRITICAL("Couldn't initialize GLAD");
    return SDL_APP_FAILURE;
  }

  SDL_GL_MakeCurrent(m_sdlWindow, m_glContext);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  SDL_ShowWindow(m_sdlWindow);

  g_imguiManager.setup();

  glViewport(0, 0, Config::Window::WIDTH, Config::Window::HEIGHT);

  SPDLOG_INFO("SDL and OpenGL initialized successfully");

  model3d.load("resources/models/cube/cube.obj");
  model3d.setupAllBuffers();

  g_camera.setActive(false);
  g_floorGrid.setup();

  return SDL_APP_CONTINUE;
}

SDL_AppResult Window::processEvent(const SDL_Event *event) {
  g_imguiManager.processEvent(event);

  g_camera.processEvent(event);

  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }

  if (event->type == SDL_EVENT_WINDOW_RESIZED) {
    const auto width = event->window.data1;
    const auto height = event->window.data2;
    glViewport(0, 0, width, height);
    return SDL_APP_CONTINUE;
  }

  if (event->type == SDL_EVENT_KEY_DOWN) {
    if (event->key.scancode == SDL_SCANCODE_TAB) {
      g_camera.toggle();
      SPDLOG_INFO("Camera toggled: {}", g_camera.isActive() ? "ON" : "OFF");
    }
  }

  return SDL_APP_CONTINUE;
}

void Window::dispose() {
  if (m_glContext) {
    if (SDL_GL_DestroyContext(m_glContext)) {
      m_glContext = nullptr;
    } else {
      SPDLOG_ERROR("Failed to destroy GL context: {}", SDL_GetError());
    }
  }

  if (m_sdlRenderer) {
    SDL_DestroyRenderer(m_sdlRenderer);
    m_sdlRenderer = nullptr;
  }

  if (m_sdlWindow) {
    SDL_DestroyWindow(m_sdlWindow);
    m_sdlWindow = nullptr;
  }
}

void Window::createImGuiWindows() {
  // Show the demo window
  static bool show_demo = true;
  ImGui::ShowDemoWindow(&show_demo);

  ImGui::SetNextWindowSizeConstraints(ImVec2(250, 250), ImVec2(FLT_MAX, FLT_MAX));
  ImGui::Begin("Engine Teaks", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("FPS: %.2f", g_imguiManager.io().Framerate);
  ImGui::Text("Camera Active: %s", g_camera.isActive() ? "Yes" : "No");
  ImGui::Text("Camera Pos: %.2f, %.2f, %.2f", g_camera.getPosition().x, g_camera.getPosition().y,
              g_camera.getPosition().z);
  ImGui::ColorEdit3("Clear Color", Config::Window::CLEAR_COLOR, ImGuiColorEditFlags_Float);
  ImGui::End();
}

void Window::renderOpenGlData() {
  const std::shared_ptr<Shader> materialShader = g_shaderCache.get("material");

  auto model = glm::mat4(1.0f);
  const glm::mat4 view = g_camera.getViewMatrix();

  const ImGuiIO &io = g_imguiManager.io();
  const auto aspectRatio = io.DisplaySize.x / io.DisplaySize.y;

  const glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

  FloorGrid::render(view, projection);

  model = glm::rotate(model, static_cast<float>(SDL_GetTicks()) / 1000.f, glm::vec3(0.5f, 0.5f, 0.0f));

  materialShader->set("u_model", model);
  materialShader->set("u_view", view);
  materialShader->set("u_projection", projection);

  materialShader->use();

  model3d.render();
}

void Window::render() const {
  g_camera.update();

  g_imguiManager.newFrame();

  createImGuiWindows();

  // Rendering
  g_imguiManager.populateFrame();

  auto [width, height] = g_imguiManager.io().DisplaySize;
  const auto [r, g, b] = Config::Window::CLEAR_COLOR;

  glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
  glClearColor(r, g, b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderOpenGlData();

  g_imguiManager.renderFrame();
  SDL_GL_SwapWindow(m_sdlWindow);
}
} // namespace App
