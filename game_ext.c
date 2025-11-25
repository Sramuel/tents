#include "game_ext.h"

#include <stdio.h>
#include <stdlib.h>

#include "game_aux.h"
#include "game_struct.h"

game game_new_ext(uint nb_rows, uint nb_cols, square* squares, uint* nb_tents_row, uint* nb_tents_col, bool wrapping) {
  return NULL;
}
game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping) { return NULL; }
uint game_nb_rows(cgame g) { return 0; }
uint game_nb_cols(cgame g) { return 0; }
bool game_is_wrapping(cgame g) { return false; }
void game_undo(game g) {}
void game_redo(game g) {}