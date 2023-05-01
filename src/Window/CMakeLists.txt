cmake_minimum_required(VERSION 3.14)

project(cinq-window CXX)

add_library(
    ${PROJECT_NAME}
    Imgui.cpp
    Mouse.cpp
    Window.cpp
    Keyboard.cpp
    Exception.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    imgui
)