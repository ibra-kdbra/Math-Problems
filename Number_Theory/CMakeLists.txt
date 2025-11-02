cmake_minimum_required(VERSION 3.16)

project(NumberTheoryGUI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts Test)

# Enable Qt6 automoc, autorcc, and autouic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/MainWindow.ui
    src/core/NumberTheoryEngine.cpp
    src/core/NumberTheoryEngine.h
    src/core/AlgorithmResult.cpp
    src/core/AlgorithmResult.h
)

# Add test executable
add_executable(${PROJECT_NAME}Test
    tests/test_mainwindow.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/MainWindow.ui
    src/core/NumberTheoryEngine.cpp
    src/core/NumberTheoryEngine.h
    src/core/AlgorithmResult.cpp
    src/core/AlgorithmResult.h
)

target_link_libraries(${PROJECT_NAME}Test PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
    Qt6::Test
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms
)

# Link Qt6 libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
)

# Set compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install targets
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# CPack configuration for packaging
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Educational Number Theory GUI Application")
set(CPACK_PACKAGE_VENDOR "Number Theory Project")
include(CPack)
