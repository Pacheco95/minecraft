#define SDL_MAIN_USE_CALLBACKS 1

#include <spdlog/spdlog.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Container.h"

using namespace App;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::trace);
  g_container.init();
  return g_window.setup();
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  return g_window.processEvent(event);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  g_time.update();
  g_window.render();

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, const SDL_AppResult result) {
  if (result == SDL_APP_SUCCESS) {
    SPDLOG_INFO("Application finished successfully");
  } else {
    SPDLOG_ERROR("Application finished with error(s): {}", SDL_GetError());
  }

  g_container.dispose();
}
