cmake_minimum_required(VERSION 3.16)
project(MinHook)

file(GLOB_RECURSE MINHOOK_SRC src/*.c)
add_library(MinHook STATIC ${MINHOOK_SRC})

target_include_directories(MinHook PUBLIC include)
