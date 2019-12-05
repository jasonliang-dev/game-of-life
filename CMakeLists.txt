cmake_minimum_required(VERSION 3.10)

project(GameOfLife)

find_package(PkgConfig)

pkg_check_modules(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
include_directories(${SDL2_IMAGE_DIRS})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(SRCS main.c game.c)
set(HEADERS constants.h game.h)

add_executable(GameOfLife ${SRCS} ${HEADERS})
target_link_libraries(GameOfLife ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
