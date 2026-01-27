#ifndef __GAME_AUX2_H__
#define __GAME_AUX2_H__

#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"

square** square_to_board_aux(game g, square* squares, uint nb_rows, uint nb_cols);
square* board_to_square_aux(cgame g, uint nb_rows, uint nb_cols);
game init_game(game g, uint* nb_tents_row, uint* nb_tents_col, square* squares);

uint nb_random(uint x);
square tree_random();
#endif
