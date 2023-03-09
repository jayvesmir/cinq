cmake_minimum_required(VERSION 3.14)

project(cinq CXX)
set(CMAKE_EXPORT_COMPILE_COMANDS on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_BUILD_TYPE "Release")
# MSVC specific arguments
set(CMAKE_CXX_FLAGS "/O2 /MT")


add_executable(
    ${PROJECT_NAME}
    WIN32
    src/Cinq.cpp
)