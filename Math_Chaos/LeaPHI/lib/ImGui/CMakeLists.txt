cmake_minimum_required(VERSION 3.0)

set(IMGUI_BASE_MODULES imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_widgets.cpp)
set(IMGUI_OPENGL3_MODULE imgui_impl_opengl3.cpp)
set(IMGUI_GLFW_MODULE imgui_impl_glfw.cpp)

add_library(ImGui ${IMGUI_BASE_MODULES} ${IMGUI_OPENGL3_MODULE} ${IMGUI_GLFW_MODULE})
