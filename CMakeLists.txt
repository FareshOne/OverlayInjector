cmake_minimum_required(VERSION 3.16)
project(OverlayInjector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Source files
file(GLOB_RECURSE SRC
    "*.cpp"
    "*.h"
)

add_library(OverlayInjector SHARED ${SRC})

# Include directories
target_include_directories(OverlayInjector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    extern/MinHook/include
    imgui
    imgui/backends
)

# MinHook
add_subdirectory(extern/MinHook)
target_link_libraries(OverlayInjector PRIVATE MinHook)

# ImGui
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_win32.cpp
    imgui/backends/imgui_impl_dx12.cpp
)
target_include_directories(imgui PUBLIC
    imgui
    imgui/backends
)
target_link_libraries(OverlayInjector PRIVATE imgui)

# Link Windows and DX12 libraries
target_link_libraries(OverlayInjector PRIVATE
    d3d12
    dxgi
    dwrite
    user32
)
