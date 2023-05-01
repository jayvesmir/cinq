cmake_minimum_required(VERSION 3.14)

project(cinq-core CXX)

add_library(
    ${PROJECT_NAME}
    Cinq.cpp
    Timer.cpp
    Image.cpp
    Camera.cpp
    Pipeline.cpp
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