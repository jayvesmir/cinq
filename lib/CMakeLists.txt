cmake_minimum_required(VERSION 3.14)

add_library(
    cinq-lib
    INTERFACE
)

add_library(
    imgui
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp

    # Building all of this seems useless... and it is.
    # But why not lol
    imgui/backends/imgui_impl_dx9.cpp
    imgui/backends/imgui_impl_dx10.cpp
    imgui/backends/imgui_impl_dx12.cpp
    # Actually using
    imgui/backends/imgui_impl_dx11.cpp
    imgui/backends/imgui_impl_win32.cpp
)

target_include_directories(
    imgui
    PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/imgui
)

target_include_directories(
    cinq-lib
    INTERFACE
    ${CMAKE_SOURCE_DIR}/lib              # stb includes
    ${CMAKE_SOURCE_DIR}/lib/imgui        # imgui includes
    ${CMAKE_SOURCE_DIR}/lib/rang/include # rang includes
)

target_link_libraries(
    cinq-lib
    INTERFACE
    imgui
    d3d11
    d3dcompiler
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    cinq-lib
)