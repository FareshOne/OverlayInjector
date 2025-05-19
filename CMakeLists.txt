cmake_minimum_required(VERSION 3.16)
project(OverlayInjector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Source files
file(GLOB_RECURSE SOURCES
    *.cpp
    *.h
)

add_library(OverlayInjector SHARED ${SOURCES})

# Include paths
target_include_directories(OverlayInjector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    extern/MinHook/include
    extern/imgui
    extern/imgui/backends
)

# MinHook
add_subdirectory(extern/MinHook)
target_include_directories(OverlayInjector PRIVATE extern/MinHook/include)
target_link_libraries(OverlayInjector PRIVATE MinHook)

# ImGui
add_library(imgui STATIC
    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/imgui_demo.cpp
    extern/imgui/backends/imgui_impl_win32.cpp
    extern/imgui/backends/imgui_impl_dx12.cpp
)

target_include_directories(imgui PRIVATE
    extern/imgui
    extern/imgui/backends
)

target_link_libraries(OverlayInjector PRIVATE imgui)

# Windows dependencies
target_link_libraries(OverlayInjector PRIVATE
    d3d12
    dxgi
    dwrite
    user32
    gdi32
    winhttp
)