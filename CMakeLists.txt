cmake_minimum_required(VERSION 3.10)
project(BitDecoder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GLFW
find_package(glfw3 REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

add_library(imgui
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Executable
add_executable(BitDecoder
    src/main.cpp
    src/TranslatorLogic.cpp
)

target_link_libraries(BitDecoder
    imgui
    glfw
    OpenGL::GL
)

