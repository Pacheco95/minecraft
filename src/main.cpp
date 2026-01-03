#define SDL_MAIN_USE_CALLBACKS 1

#include <spdlog/spdlog.h>

#include <glad/glad.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Time.h"
#include "Window.h"

using namespace App;

static Window window;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::trace);
  *appstate = &window;
  return window.setup();
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  return window.processEvent(event);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  Time::update();
  window.render();
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, const SDL_AppResult result) {
  if (result == SDL_APP_SUCCESS) {
    SPDLOG_INFO("Application finished successfully");
  } else {
    SPDLOG_ERROR("Application finished with error(s): {}", SDL_GetError());
  }

  window.dispose();
}
