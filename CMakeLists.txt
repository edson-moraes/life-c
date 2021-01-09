cmake_minimum_required(VERSION 3.17)
project(life-c C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(life-c main.c)

target_link_libraries(life-c ${SDL2_LIBRARIES})
