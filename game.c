#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "game_struct.h"

game game_new(square* squares, uint* nb_tents_row, uint* nb_tents_col) { return NULL; }
game game_new_empty(void) { return NULL; }
game game_copy(cgame g) { return NULL; }
bool game_equal(cgame g1, cgame g2) { return false; }
void game_delete(game g) {}
void game_set_square(game g, uint i, uint j, square s) {}
square game_get_square(cgame g, uint i, uint j) { return EMPTY; }
void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents) {}
void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents) {}
uint game_get_expected_nb_tents_row(cgame g, uint i) { return 0; }
uint game_get_expected_nb_tents_col(cgame g, uint j) { return 0; }
uint game_get_expected_nb_tents_all(cgame g) { return 0; }
uint game_get_current_nb_tents_row(cgame g, uint i) { return 0; }
uint game_get_current_nb_tents_col(cgame g, uint j) { return 0; }
uint game_get_current_nb_tents_all(cgame g) { return 0; }
void game_play_move(game g, uint i, uint j, square s) {}
bool game_check_move(cgame g, uint i, uint j, square s) { return false; }
bool game_won(cgame g) { return false; }
void game_fill_grass(game g) {}
void game_restart(game g) {}