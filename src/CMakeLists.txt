cmake_minimum_required(VERSION 3.14)

set(RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/res)
set(SHADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/shader)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-O4")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-O5")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/O2 /MT /MP /EHsc")
    set(RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/res)
    set(SHADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shader)
endif()

add_executable(
    ${PROJECT_NAME}
    WIN32
    ../res/Cinq.rc
    Cinq.cpp
    Mouse.cpp
    Image.cpp
    timer.cpp
    Window.cpp
    Ayanami.cpp
    Pipeline.cpp
    Keyboard.cpp
    Exception.cpp
)

add_subdirectory(Bindable)
add_subdirectory(Drawable)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    d3d11
    d3dcompiler

    cinq-bindable
    cinq-drawable
)

# Build HLSL shaders
add_custom_target(shaders)
file(MAKE_DIRECTORY ${SHADER_DIR})
set(
    SHADERS
    Shader/TextureVS.hlsl
    Shader/TexturePS.hlsl
    Shader/ColorIndexVS.hlsl
    Shader/ColorIndexPS.hlsl
    Shader/ColorBlendVS.hlsl
    Shader/ColorBlendPS.hlsl
)

set_source_files_properties(Shader/TextureVS.hlsl PROPERTIES shaderType "vs")
set_source_files_properties(Shader/TexturePS.hlsl PROPERTIES shaderType "ps")
set_source_files_properties(Shader/ColorIndexVS.hlsl PROPERTIES shaderType "vs")
set_source_files_properties(Shader/ColorIndexPS.hlsl PROPERTIES shaderType "ps")
set_source_files_properties(Shader/ColorBlendVS.hlsl PROPERTIES shaderType "vs")
set_source_files_properties(Shader/ColorBlendPS.hlsl PROPERTIES shaderType "ps")
set_source_files_properties(${SHADERS} PROPERTIES shaderModel "4_0")

foreach(FILE ${SHADERS})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    get_source_file_property(shadertype ${FILE} shaderType)
    get_source_file_property(shadermodel ${FILE} shaderModel)
    add_custom_command(TARGET shaders
                       COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} /O2 /Zi /Fo ${SHADER_DIR}/${FILE_WE}.cso /I ${CMAKE_SOURCE_DIR}/src/Shader ${FILE}
                       MAIN_DEPENDENCY ${FILE}
                       COMMENT "HLSL ${FILE}"
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       VERBATIM)
endforeach(FILE)

# Copy resources
add_custom_target(resources)
file(MAKE_DIRECTORY ${RESOURCE_DIR})
set(
    RESOURCES
    ${CMAKE_SOURCE_DIR}/res/2b.png
    ${CMAKE_SOURCE_DIR}/res/cube.png
)

foreach(FILE ${RESOURCES})
    add_custom_command(TARGET resources
                       COMMAND ${CMAKE_COMMAND}
                       ARGS -E copy ${FILE} ${RESOURCE_DIR}
                       COMMENT "Copy ${FILE}"
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       VERBATIM)
endforeach(FILE)

add_dependencies(
    ${PROJECT_NAME}
    shaders
    resources
)