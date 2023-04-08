cmake_minimum_required(VERSION 3.14)

project(cinq-bindable CXX)
set(CMAKE_EXPORT_COMPILE_COMANDS on)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_BUILD_TYPE "Release")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-O4")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-O5")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/O2 /MT /MP /EHsc")
endif()

add_library(
    ${PROJECT_NAME}
    Topology.cpp
    IndexBuffer.cpp
    InputLayout.cpp
    PixelShader.cpp
    VertexBuffer.cpp
    VertexShader.cpp
    TransformCBuffer.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)