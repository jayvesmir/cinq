cmake_minimum_required(VERSION 3.14)

project(cinq CXX)
set(CMAKE_EXPORT_COMPILE_COMANDS on)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_BUILD_TYPE "Release")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-O3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-O5")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/O2 /MT /MP /EHsc")
endif()

# build HLSL shaders
# build directory is kinda nonsensical
add_custom_target(shaders)
set(SHADER_DIR src/Shader)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shader)
set(SHADERS ${SHADER_DIR}/vertex.hlsl ${SHADER_DIR}/pixel.hlsl)
set_source_files_properties(${SHADER_DIR}/vertex.hlsl PROPERTIES shaderType "vs")
set_source_files_properties(${SHADER_DIR}/pixel.hlsl PROPERTIES shaderType "ps")
set_source_files_properties(${SHADERS} PROPERTIES shaderModel "4_0")

foreach(FILE ${SHADERS})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    get_source_file_property(shadertype ${FILE} shaderType)
    get_source_file_property(shadermodel ${FILE} shaderModel)
    add_custom_command(TARGET shaders
                       COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} /O2 /Zi /Fo ${CMAKE_SOURCE_DIR}/shader/${FILE_WE}.cso ${FILE}
                       MAIN_DEPENDENCY ${FILE}
                       COMMENT "HLSL ${FILE}"
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       VERBATIM)
endforeach(FILE)

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

    src/Bindable/Topology.cpp
    src/Bindable/IndexBuffer.cpp
    src/Bindable/InputLayout.cpp
    src/Bindable/PixelShader.cpp
    src/Bindable/VertexBuffer.cpp
    src/Bindable/VertexShader.cpp
    src/Bindable/TransformCBuffer.cpp

    src/Drawable/Cube.cpp
    src/Drawable/Drawable.cpp
)

add_dependencies(
    ${PROJECT_NAME}
    shaders
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    d3d11
    d3dcompiler
)