cmake_minimum_required(VERSION 4.1.2)
project(Minecraft)

# Include FetchContent module for downloading dependencies
include(FetchContent)

# ========================= BEGIN DEPENDENCIES =========================

set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(SDL_TEST_LIBRARY OFF)
set(SDL_TESTS OFF)
set(SDL_DISABLE_INSTALL ON)
set(SDL_DISABLE_INSTALL_DOCS ON)
set(SDL_INSTALL_TESTS ON)
set(SDL_AUDIO OFF)
set(SDL_CAMERA OFF)
set(SDL_HAPTIC OFF)
set(SDL_SENSOR OFF)
set(SDL_CAMERA OFF)
set(SDL_RENDERER_VULKAN OFF)
set(SDL_VULKAN OFF)

# SDL3
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.x
    FIND_PACKAGE_ARGS NAMES SDL3
)

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.16.0
)

set(SPDLOG_FMT_EXTERNAL OFF)
FetchContent_MakeAvailable(SDL3 spdlog)

# ========================= END DEPENDENCIES   =========================

add_executable(Minecraft src/main.cpp
        src/Constants.h
        src/Engine.cpp
        src/Engine.h
        src/Window.h)

target_link_libraries(Minecraft SDL3::SDL3 spdlog::spdlog)

set_target_properties(Minecraft PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)


# Link SDL3 to the executable
target_link_libraries(Minecraft SDL3::SDL3)
