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

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    cinq-lib
)