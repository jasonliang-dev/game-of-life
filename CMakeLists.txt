cmake_minimum_required(VERSION 3.7)

project(GameOfLife)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(GameOfLife main.c)
target_link_libraries(GameOfLife ${SDL2_LIBRARIES})
