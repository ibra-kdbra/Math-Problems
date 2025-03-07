cmake_minimum_required(VERSION 3.20)

project(imgui-template C CXX)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake) 

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(imgui REQUIRED)
find_package(implot REQUIRED)
find_package(exprtk REQUIRED)
find_package(OpenGL REQUIRED)

add_compile_options(-fPIC)

add_executable(
    imgui-template
    main.cpp
    bindings/imgui_impl_glfw.cpp
    bindings/imgui_impl_glfw.h
    bindings/imgui_impl_opengl3.cpp
    bindings/imgui_impl_opengl3.h
    bindings/imgui_impl_opengl3_loader.h
    )

target_link_libraries(imgui-template
    glfw
    GLEW::GLEW
    imgui::imgui
    implot::implot
    exprtk::exprtk
    ${OPENGL_gl_LIBRARY}
)
