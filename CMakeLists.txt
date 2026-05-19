cmake_minimum_required(VERSION 3.15)
project(tetris C)

set(CMAKE_C_STANDARD 11)

add_executable(tetris
    tetris/main.c
    tetris/block.c
    tetris/console.c
    tetris/game.c
)

target_include_directories(tetris PRIVATE tetris)
