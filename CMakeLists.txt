cmake_minimum_required(VERSION 4.1.2)
project(Minecraft)
include_directories(include)

file(CREATE_LINK "${PROJECT_SOURCE_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources" SYMBOLIC)

# Include FetchContent module for downloading dependencies
include(FetchContent)

# ========================= BEGIN DEPENDENCIES =========================

# Find OpenGL and make it available to targets
find_package(OpenGL REQUIRED)

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
set(SDL_OPENGL ON)
set(SDL_RENDERER_OPENGL ON)

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

# ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.5
)

FetchContent_MakeAvailable(imgui)

# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.3
)
FetchContent_MakeAvailable(glm)

# Configure ASSIMP build options
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable ASSIMP tests")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Disable ASSIMP tools")
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable ASSIMP install")
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "Disable PDB install")
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "Disable ASSIMP samples")

FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v6.0.2
)
FetchContent_MakeAvailable(assimp)

FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# ========================= END DEPENDENCIES   =========================

add_executable(Minecraft
        src/vendor/glad.c
        src/vendor/ImageLoader.cpp
        src/main.cpp
        src/Config.h

        src/Window.h
        src/Window.cpp
        src/Shader.h
        src/Shader.cpp
        src/Model.h
        src/Model.cpp
        src/Texture2D.h
        src/Texture2D.cpp
        src/Camera.h
        src/Camera.cpp
        src/Time.cpp
        src/Time.h
        src/FloorGrid.cpp
        src/FloorGrid.h
        src/ImGuiManager.h
        src/Container.h
        src/ImGuiManager.cpp
        src/Axis.cpp
        src/Axis.h
        src/MapUtils.h
        src/Cache.h
)

target_link_libraries(Minecraft SDL3::SDL3 spdlog::spdlog OpenGL::GL assimp)

target_compile_definitions(Minecraft PRIVATE "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE")

# Compile ImGui sources into the executable (includes SDL3 + OpenGL3 backends)
target_sources(Minecraft PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(Minecraft PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${glm_SOURCE_DIR}
    ${assimp_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/_deps/assimp-build/include
    ${stb_SOURCE_DIR}
)

set_target_properties(Minecraft PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

