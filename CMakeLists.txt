cmake_minimum_required(VERSION 3.10)

project(GameOfLife)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_DIRS})

add_executable(GameOfLife WIN32 main.c)
target_link_libraries(GameOfLife ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
