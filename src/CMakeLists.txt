cmake_minimum_required(VERSION 3.14)

set(SHADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/shader)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-O4")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-O5")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if (CMAKE_BUILD_TYPE STREQUAL "Release") 
        set(SHADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/Release/shader)
    elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SHADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/Debug/shader)
    endif()
    set(CMAKE_CXX_FLAGS "/O2 /MT /MP /EHsc")
endif()

add_executable(
    ${PROJECT_NAME}
    WIN32
    ../res/Cinq.rc
    Cinq.cpp
    Mouse.cpp
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

# build HLSL shaders
add_custom_target(shaders)
file(MAKE_DIRECTORY ${SHADER_DIR})
set(
    SHADERS

    Shader/ColorIndexVS.hlsl
    Shader/ColorIndexPS.hlsl
    Shader/ColorBlendVS.hlsl
    Shader/ColorBlendPS.hlsl
)

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
                       COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} /O2 /Zi /Fo ${SHADER_DIR}/${FILE_WE}.cso ${FILE}
                       MAIN_DEPENDENCY ${FILE}
                       COMMENT "HLSL ${FILE}"
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       VERBATIM)
endforeach(FILE)

add_dependencies(
    ${PROJECT_NAME}
    shaders
)