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
target_link_libraries(game_test_amassie game)

add_test(test_ansicot_dummy ./game_test_ansicot dummy)
add_test(test_ansicot_new ./game_test_ansicot new)
add_test(test_ansicot_new_empty ./game_test_ansicot new_empty)
add_test(test_ansicot_copy ./game_test_ansicot copy)
add_test(test_ansicot_equal ./game_test_ansicot equal)
add_test(test_ansicot_set_square ./game_test_ansicot set_square)
add_test(test_ansicot_get_square ./game_test_ansicot get_square)
add_test(test_ansicot_set_expected_nb_tents_row ./game_test_ansicot set_expected_nb_tents_row)
add_test(test_ansicot_set_expected_nb_tents_col ./game_test_ansicot set_expected_nb_tents_col)

add_test(test_sguillaumin_dummy ./game_test_sguillaumin dummy)
add_test(test_sguillaumin_print ./game_test_sguillaumin print)
add_test(test_sguillaumin_delete ./game_test_sguillaumin delete)
add_test(test_sguillaumin_get_expected_nb_tents_row ./game_test_sguillaumin get_expected_nb_tents_row)
add_test(test_sguillaumin_get_expected_nb_tents_col ./game_test_sguillaumin get_expected_nb_tents_col)
add_test(test_sguillaumin_get_expected_nb_tents_all ./game_test_sguillaumin get_expected_nb_tents_all)
add_test(test_sguillaumin_get_current_nb_tents_col ./game_test_sguillaumin get_current_nb_tents_col)
add_test(test_sguillaumin_get_current_nb_tents_row ./game_test_sguillaumin get_current_nb_tents_row)
add_test(test_sguillaumin_get_current_nb_tents_all ./game_test_sguillaumin get_current_nb_tents_all)
add_test(test_sguillaumin_play_move ./game_test_sguillaumin play_move)

add_test(test_amassie_dummy ./game_test_amassie dummy)
add_test(test_amassie_game_is_neigh ./game_test_amassie game_is_neigh)
add_test(test_amassie_game_get_ajacent_square ./game_test_amassie game_get_ajacent_square)
add_test(test_amassie_game_default_solution ./game_test_amassie game_default_solution)
add_test(test_amassie_game_default ./game_test_amassie game_default)
add_test(test_amassie_game_restart ./game_test_amassie game_restart)
add_test(test_amassie_game_fill_grass ./game_test_amassie game_fill_grass)
add_test(test_amassie_game_won ./game_test_amassie game_won)
add_test(test_amassie_game_check_move ./game_test_amassie game_check_move)



