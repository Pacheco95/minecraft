#include "ImGuiManager.h"

#include "Container.h"
#include "Config.h"

App::ImGuiManager::~ImGuiManager() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
}

void App::ImGuiManager::setup() {
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
  ImGui_ImplSDL3_InitForOpenGL(SDL_GL_GetCurrentWindow(), SDL_GL_GetCurrentContext());
  ImGui_ImplOpenGL3_Init(Config::Core::GLSL_VERSION);
}

void App::ImGuiManager::newFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
}

void App::ImGuiManager::populateFrame() {
  // Show the demo window
  static bool show_demo = true;
  ImGui::ShowDemoWindow(&show_demo);

  ImGui::SetNextWindowSizeConstraints(ImVec2(250, 250), ImVec2(FLT_MAX, FLT_MAX));
  ImGui::Begin("Engine Teaks", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing);
  ImGui::Text("FPS: %.2f", io().Framerate);
  ImGui::Text("Camera Pos: %.2f, %.2f, %.2f", g_camera.getPosition().x, g_camera.getPosition().y,
              g_camera.getPosition().z);
  ImGui::ColorEdit3("Clear Color", Config::Window::CLEAR_COLOR, ImGuiColorEditFlags_Float);
  ImGui::End();

  ImGui::Render();
}

void App::ImGuiManager::renderFrame() {
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void App::ImGuiManager::processEvent(const SDL_Event *event) {
  ImGui_ImplSDL3_ProcessEvent(event);
}

const ImGuiIO &App::ImGuiManager::io() const {
  return ImGui::GetIO();
}
