#pragma once

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <imgui.h>

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Config.h"

namespace App {
struct Window {
  SDL_Window *m_sdlWindow = nullptr;
  SDL_Renderer *m_sdlRenderer = nullptr;
  SDL_GLContext m_glContext = nullptr;

  SDL_AppResult setup();
  SDL_AppResult processEvent(const SDL_Event *event);
  void dispose();
  static void renderImGuiWindows();
  void render() const;

private:
  std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("bootstrap");

  void setupImgui() const;
};

inline SDL_AppResult Window::setup() {
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
  SDL_ShowWindow(m_sdlWindow);

  setupImgui();

  glViewport(0, 0, Config::Window::WIDTH, Config::Window::HEIGHT);

  SPDLOG_INFO("SDL and OpenGL initialized successfully");

  return SDL_APP_CONTINUE;
}

inline SDL_AppResult Window::processEvent(const SDL_Event *event) {
  ImGui_ImplSDL3_ProcessEvent(event);

  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }

  if (event->type == SDL_EVENT_WINDOW_RESIZED) {
    const auto width = event->window.data1;
    const auto height = event->window.data2;
    glViewport(0, 0, width, height);
    SPDLOG_DEBUG("Window resized: {} x {}", width, height);
    return SDL_APP_CONTINUE;
  }

  return SDL_APP_CONTINUE;
}

inline void Window::dispose() {
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
inline void Window::renderImGuiWindows() {
  // Show the demo window
  static bool show_demo = true;
  ImGui::ShowDemoWindow(&show_demo);

  ImGui::SetNextWindowSizeConstraints(ImVec2(250, 250), ImVec2(FLT_MAX, FLT_MAX));
  ImGui::Begin("Engine Teaks", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
  ImGui::ColorEdit3("Clear Color", Config::Window::CLEAR_COLOR, ImGuiColorEditFlags_Float);
  ImGui::End();
}

inline void Window::render() const {
  const float red = Config::Window::CLEAR_COLOR[0];
  const float green = Config::Window::CLEAR_COLOR[1];
  const float blue = Config::Window::CLEAR_COLOR[2];

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  renderImGuiWindows();

  // Rendering
  ImGui::Render();
  const ImGuiIO &io = ImGui::GetIO();
  (void)io;
  glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
  glClearColor(red, green, blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(m_sdlWindow);
}

inline void Window::setupImgui() const {
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
