cmake_minimum_required(VERSION 3.10)
project(lab02)

set(CMAKE_C_STANDARD 11)

add_executable(lab02 main.c search.c)
target_compile_definitions(lab02 PRIVATE __FILE_PREFIX__="${CMAKE_CURRENT_SOURCE_DIR}/")
