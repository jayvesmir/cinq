cmake_minimum_required(VERSION 3.14)

add_library(
    imgui
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp

    # Building all of this seems useless and it is.
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
    PUBLIC
    ${CMAKE_SOURCE_DIR}/lib/imgui
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    imgui
)