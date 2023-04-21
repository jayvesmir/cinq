cmake_minimum_required(VERSION 3.14)

project(cinq-drawable CXX)

add_library(
    ${PROJECT_NAME}
    Cube.cpp
    Plane.cpp
    Sphere.cpp
    Pyramid.cpp
    Drawable.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    imgui
)