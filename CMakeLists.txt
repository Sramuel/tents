cmake_minimum_required(VERSION 3.5)

project(game C)

include(CTest)
enable_testing()

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

link_directories(${CMAKE_SOURCE_DIR})
add_executable(game_text game_text.c)
add_executable(game_test_ansicot game_test_ansicot.c)
target_link_libraries(game_text game)

add_test(test_ansicot_dummy ./game_test_ansicot dummy)