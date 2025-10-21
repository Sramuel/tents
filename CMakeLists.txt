cmake_minimum_required(VERSION 3.5)

project(game C)

include(CTest)
enable_testing()

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

link_directories(${CMAKE_SOURCE_DIR})
add_executable(game_text game_text.c)

add_executable(game_test_sguillaumin game_test_sguillaumin.c)
add_executable(game_test_ansicot game_test_ansicot.c)
add_executable(game_test_amassie game_test_amassie.c)

target_link_libraries(game_text game)
target_link_libraries(game_test_sguillaumin game)
target_link_libraries(game_test_ansicot game)

add_test(test_ansicot_dummy ./game_test_ansicot dummy)
add_test(test_ansicot_new ./game_test_ansicot new)

add_test(test_sguillaumin_dummy ./game_test_sguillaumin dummy)
add_test(test_sguillaumin_print ./game_test_sguillaumin print)
add_test(test_sguillaumin_delete ./game_test_sguillaumin delete)

add_test(test_amassie_dummy ./game_test_amassie dummy)