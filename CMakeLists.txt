cmake_minimum_required(VERSION 3.17)
project(life-c C)

set(CMAKE_C_STANDARD 11)
FILE(COPY spotlight-big-alpha4.bmp DESTINATION "${CMAKE_BINARY_DIR}")
add_compile_options(-Wall -Wextra -Wshadow -pedantic)

find_package(X11 REQUIRED)

message(STATUS "X11_FOUND = ${X11_FOUND}")
message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

include_directories(${X11_INCLUDE_DIR})

link_directories(${X11_LIBRARIES})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(life-c src/main.c src/renderer.h src/renderer.c src/game.c src/game.h)

target_link_libraries(life-c ${SDL2_LIBRARIES} ${X11_LIBRARIES})
