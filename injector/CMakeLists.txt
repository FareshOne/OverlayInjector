cmake_minimum_required(VERSION 3.16)
project(OverlayInjectorLauncher)

add_executable(OverlayInjectorLauncher Injector.cpp)
set_target_properties(OverlayInjectorLauncher PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_compile_definitions(OverlayInjectorLauncher PRIVATE UNICODE _UNICODE)
