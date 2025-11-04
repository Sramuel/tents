#include "game_aux.h"

#include <stdio.h>

#include "game_struct.h"

void game_print(cgame g) {}
game game_default(void) { return NULL; }
game game_default_solution(void) { return NULL; }
bool game_get_ajacent_square(cgame g, uint i, uint j, direction d, uint* pi_next, uint* pj_next) { return false; }
bool game_is_neigh(cgame g, uint i, uint j, square s, bool diag) { return false; }