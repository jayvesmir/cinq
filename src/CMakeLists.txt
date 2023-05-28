cmake_minimum_required(VERSION 3.14)

set(RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/res)
set(SHADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/shader)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/res)
    set(SHADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shader)
endif()

add_executable(
    ${PROJECT_NAME}
    WIN32
    Ayanami.cpp
    ../res/Cinq.rc
)

add_subdirectory(Core)
add_subdirectory(Window)
add_subdirectory(Bindable)
add_subdirectory(Drawable)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    cinq-lib
    cinq-core
    cinq-window
    cinq-bindable
    cinq-drawable
)

# Build HLSL shaders
add_custom_target(shaders)
file(MAKE_DIRECTORY ${SHADER_DIR})
set(
    SHADERS
    Shader/SolidVS.hlsl
    Shader/SolidPS.hlsl
    Shader/PhongVS.hlsl
    Shader/PhongPS.hlsl
    Shader/TextureVS.hlsl
    Shader/TexturePS.hlsl
    Shader/ColorIndexVS.hlsl
    Shader/ColorIndexPS.hlsl
    Shader/ColorBlendVS.hlsl
    Shader/ColorBlendPS.hlsl
)

set_source_files_properties(Shader/SolidVS.hlsl PROPERTIES shaderType "vs")
set_source_files_properties(Shader/PhongVS.hlsl PROPERTIES shaderType "vs")
set_source_files_properties(Shader/TextureVS.hlsl PROPERTIES shaderType "vs")
set_source_files_properties(Shader/ColorIndexVS.hlsl PROPERTIES shaderType "vs")
set_source_files_properties(Shader/ColorBlendVS.hlsl PROPERTIES shaderType "vs")
set_source_files_properties(Shader/SolidPS.hlsl PROPERTIES shaderType "ps")
set_source_files_properties(Shader/PhongPS.hlsl PROPERTIES shaderType "ps")
set_source_files_properties(Shader/TexturePS.hlsl PROPERTIES shaderType "ps")
set_source_files_properties(Shader/ColorIndexPS.hlsl PROPERTIES shaderType "ps")
set_source_files_properties(Shader/ColorBlendPS.hlsl PROPERTIES shaderType "ps")
set_source_files_properties(${SHADERS} PROPERTIES shaderModel "5_0")

foreach(FILE ${SHADERS})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    get_source_file_property(shadertype ${FILE} shaderType)
    get_source_file_property(shadermodel ${FILE} shaderModel)
    add_custom_command(TARGET shaders
                       COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} /O3 /Fo ${SHADER_DIR}/${FILE_WE}.cso /I ${CMAKE_SOURCE_DIR}/src/Shader ${FILE}
                       MAIN_DEPENDENCY ${FILE}
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
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       VERBATIM)
endforeach(FILE)

add_dependencies(
    ${PROJECT_NAME}
    shaders
    resources
)