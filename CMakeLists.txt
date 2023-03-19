cmake_minimum_required(VERSION 3.14)

project(cinq CXX)
set(CMAKE_EXPORT_COMPILE_COMANDS on)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_BUILD_TYPE "Release")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-O3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-O5")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/O2 /MT /MP /EHsc")
endif()

add_executable(
    ${PROJECT_NAME}
    WIN32
    res/Cinq.rc
    src/Cinq.cpp
    src/Mouse.cpp
    src/timer.cpp
    src/Window.cpp
    src/Ayanami.cpp
    src/Pipeline.cpp
    src/Keyboard.cpp
    src/Exception.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    d3d11.lib
)