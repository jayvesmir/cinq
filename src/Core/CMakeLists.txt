cmake_minimum_required(VERSION 3.14)

project(cinq-core CXX)

add_library(
    ${PROJECT_NAME}
    Cinq.cpp
    Timer.cpp
    Image.cpp
    Camera.cpp
    Logger.cpp
    Pipeline.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    cinq-lib
)