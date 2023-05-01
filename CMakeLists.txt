cmake_minimum_required(VERSION 3.14)

project(cinq CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMANDS on)

include_directories(
    ${CMAKE_SOURCE_DIR}/lib
)

add_subdirectory(src)
add_subdirectory(lib)