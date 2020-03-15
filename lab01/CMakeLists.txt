cmake_minimum_required(VERSION 3.10)
project(lab01)

set(CMAKE_C_STANDARD 11)

add_executable(lab01 main.c tree.c sort.c task.c)
target_compile_definitions(lab01 PRIVATE __FILE_PREFIX__="${CMAKE_CURRENT_SOURCE_DIR}/")
