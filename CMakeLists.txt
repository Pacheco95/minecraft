cmake_minimum_required(VERSION 4.2)

project(Minecraft)

add_executable(Minecraft src/main.cpp)

set_target_properties(Minecraft PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
