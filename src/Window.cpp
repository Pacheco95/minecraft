#include "Window.h"

#include <memory>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <imgui.h>

#include "Camera.h"
#include "Config.h"
#include "Model.h"
#include "Shader.h"
#include "ShaderCache.h"
#include "Sphere.h"

namespace App {

Model model3d;
Camera camera;

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

  const auto loader = reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress);

  if (!gladLoadGLLoader(loader)) {
    logger->critical("Couldn't initialize GLAD");
    return SDL_APP_FAILURE;
  }

  SDL_GL_MakeCurrent(m_sdlWindow, m_glContext);
  SDL_GL_SetSwapInterval(1); // Enable vsync
  glEnable(GL_DEPTH_TEST);

  SDL_ShowWindow(m_sdlWindow);

  setupImgui();

  glViewport(0, 0, Config::Window::WIDTH, Config::Window::HEIGHT);

  SPDLOG_INFO("SDL and OpenGL initialized successfully");

  model3d.load("resources/models/cube/cube.obj");
  model3d.setupAllBuffers();

  camera.setActive(false);

  return SDL_APP_CONTINUE;
}

SDL_AppResult Window::processEvent(const SDL_Event *event) {
  ImGui_ImplSDL3_ProcessEvent(event);

  camera.processEvent(event);

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
      camera.setActive(!camera.isActive());
      SPDLOG_INFO("Camera toggled: {}", camera.isActive() ? "ON" : "OFF");
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

  // Shutdown ImGui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
}

void Window::createImGuiWindows() {
  // Show the demo window
  static bool show_demo = true;
  ImGui::ShowDemoWindow(&show_demo);

  ImGui::SetNextWindowSizeConstraints(ImVec2(250, 250), ImVec2(FLT_MAX, FLT_MAX));
  ImGui::Begin("Engine Teaks", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
  ImGui::Text("Camera Active: %s", camera.isActive() ? "Yes" : "No");
  ImGui::Text("Camera Pos: %.2f, %.2f, %.2f", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
  ImGui::ColorEdit3("Clear Color", Config::Window::CLEAR_COLOR, ImGuiColorEditFlags_Float);
  ImGui::End();
}

void Window::renderScene() {
  const std::shared_ptr<Shader> materialShader = ShaderCache::get("material");

  auto model = glm::mat4(1.0f);
  const glm::mat4 view = camera.getViewMatrix();
  const glm::mat4 projection =
      glm::perspective(glm::radians(45.0f), (float)Config::Window::WIDTH / (float)Config::Window::HEIGHT, 0.1f, 100.0f);

  model = glm::rotate(model, SDL_GetTicks() / 1000.f, glm::vec3(0.5f, 0.5f, 0.0f));

  materialShader->set("u_model", model);
  materialShader->set("u_view", view);
  materialShader->set("u_projection", projection);

  materialShader->use();

  model3d.render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::render() const {
  camera.update(); // Update camera with fixed timestep

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  createImGuiWindows();

  // Rendering
  ImGui::Render();
  const ImGuiIO &io = ImGui::GetIO();
  glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));

  const auto [r, g, b] = Config::Window::CLEAR_COLOR;
  glClearColor(r, g, b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderScene();

  SDL_GL_SwapWindow(m_sdlWindow);
}

void Window::setupImgui() const {
  const float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  // Setup scaling
  ImGuiStyle &style = ImGui::GetStyle();

  // Bake a fixed style scale. (until we have a solution for
  // dynamic style scaling, changing this requires resetting
  // Style + calling this again)
  style.ScaleAllSizes(mainScale);

  // Set initial font scale. (using io.ConfigDpiScaleFonts=true
  // makes this unnecessary. We leave both here for documentation purpose)
  style.FontScaleDpi = mainScale;

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForOpenGL(m_sdlWindow, m_glContext);
  ImGui_ImplOpenGL3_Init("#version 330");
}
} // namespace App
