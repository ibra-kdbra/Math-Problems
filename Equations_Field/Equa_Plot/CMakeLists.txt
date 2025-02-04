cmake_minimum_required(VERSION 3.20)

project(imgui-template C CXX)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake) 

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)

add_compile_options(-fPIC)  

add_subdirectory(dependency)

add_executable(imgui-template main.cpp)

target_link_libraries(imgui-template 
    glfw
    GLEW::GLEW
    imgui 
    implot 
    exprtk
)

