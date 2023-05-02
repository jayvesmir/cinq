cmake_minimum_required(VERSION 3.14)

project(cinq-bindable CXX)

add_library(
    ${PROJECT_NAME}
    Sampler.cpp
    Texture.cpp
    Topology.cpp
    IndexBuffer.cpp
    InputLayout.cpp
    PixelShader.cpp
    VertexBuffer.cpp
    VertexShader.cpp
    TransformCBuffer.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    cinq-lib
)