cmake_minimum_required(VERSION 3.5)

project(game C)

include(CTest)
enable_testing()

set(CMAKE_C_FLAGS "-std=c99 -g -Wall -O3")

## find SDL2
include(sdl2.cmake)
include_directories(${SDL2_ALL_INC})

add_library(game game.c game_aux.c  game_aux2.c game_ext.c game_tools.c queue.c)

add_executable(game_text game_text.c)

add_executable(game_random game_random.c)

add_executable(game_solve game_solve.c)

add_executable(game_sdl game_sdl.c game_sdl_model.c)

add_executable(game_test_student game_test_student.c)
add_executable(game_test_student2 game_test_student2.c)
add_executable(game_test_student3 game_test_student3.c)
add_executable(game_test_tools game_test_tools.c)

target_link_libraries(game_text game)
target_link_libraries(game_random game)
target_link_libraries(game_solve game)
target_link_libraries(game_sdl game ${SDL2_ALL_LIBS})

target_link_libraries(game_test_student game)
target_link_libraries(game_test_student2 game)
target_link_libraries(game_test_student3 game)
target_link_libraries(game_test_tools game)

add_test(test_student2_dummy ./game_test_student2 dummy)
add_test(test_student2_new ./game_test_student2 new)
add_test(test_student2_new_empty ./game_test_student2 new_empty)
add_test(test_student2_copy ./game_test_student2 copy)
add_test(test_student2_equal ./game_test_student2 equal)
add_test(test_student2_set_square ./game_test_student2 set_square)
add_test(test_student2_get_square ./game_test_student2 get_square)
add_test(test_student2_set_expected_nb_tents_row ./game_test_student2 set_expected_nb_tents_row)
add_test(test_student2_set_expected_nb_tents_col ./game_test_student2 set_expected_nb_tents_col)
add_test(test_student2_new_ext ./game_test_student2 new_ext)
add_test(test_student2_new_empty_ext ./game_test_student2 new_empty_ext)

add_test(test_student_dummy ./game_test_student dummy)
add_test(test_student_print ./game_test_student print)
add_test(test_student_delete ./game_test_student delete)
add_test(test_student_get_expected_nb_tents_row ./game_test_student get_expected_nb_tents_row)
add_test(test_student_get_expected_nb_tents_col ./game_test_student get_expected_nb_tents_col)
add_test(test_student_get_expected_nb_tents_all ./game_test_student get_expected_nb_tents_all)
add_test(test_student_get_current_nb_tents_col ./game_test_student get_current_nb_tents_col)
add_test(test_student_get_current_nb_tents_row ./game_test_student get_current_nb_tents_row)
add_test(test_student_get_current_nb_tents_all ./game_test_student get_current_nb_tents_all)
add_test(test_student_play_move ./game_test_student play_move)
add_test(test_student_game_undo ./game_test_student game_undo)
add_test(test_student_game_redo ./game_test_student game_redo)

add_test(test_student3_dummy ./game_test_student3 dummy)
add_test(test_student3_game_is_neigh ./game_test_student3 game_is_neigh)
add_test(test_student3_game_get_adjacent_square ./game_test_student3 game_get_adjacent_square)
add_test(test_student3_game_default_solution ./game_test_student3 game_default_solution)
add_test(test_student3_game_default ./game_test_student3 game_default)
add_test(test_student3_game_restart ./game_test_student3 game_restart)
add_test(test_student3_game_fill_grass ./game_test_student3 game_fill_grass)
add_test(test_student3_game_won ./game_test_student3 game_won)
add_test(test_student3_game_check_move ./game_test_student3 game_check_move)
add_test(test_student3_game_nb_neigh ./game_test_student3 game_nb_neigh)
add_test(test_student3_game_nb_rows ./game_test_student3 game_nb_rows)
add_test(test_student3_game_nb_cols ./game_test_student3 game_nb_cols)
add_test(test_student3_game_is_wrapping ./game_test_student3 game_is_wrapping)

add_test(test_tools_game_save ./game_test_tools game_save)
add_test(test_tools_game_load ./game_test_tools game_load)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/levels/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/levels/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res/)