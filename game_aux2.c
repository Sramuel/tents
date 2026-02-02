#include "game_aux2.h"

#include <stdio.h>
#include <stdlib.h>

#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"

//////////////////////////////////////////////////////////////////////////AUXILIAIRES/////////////////////////////////////////////////////////////////////////////

square** square_to_board_aux(game g, square* squares, uint nb_rows, uint nb_cols) {
  for (uint i = 0; i < game_nb_rows(g); i++) {
    for (uint j = 0; j < game_nb_cols(g); j++) {
      game_set_square(g, i, j,
                      squares[i * nb_cols + j]);  // Squares est un simple tableau, il faut alors traduire ces indices
    }
  }
  return g->board;
}

square* board_to_square_aux(cgame g, uint nb_rows, uint nb_cols) {
  square* squares = malloc(nb_cols * nb_rows * sizeof(square));
  if (squares == NULL) {
    exit(EXIT_FAILURE);
  }
  for (uint i = 0; i < game_nb_rows(g); i++) {
    for (uint j = 0; j < game_nb_cols(g); j++) {
      squares[i * nb_cols + j] =
          game_get_square(g, i, j);  // Squares est un simple tableau, il faut alors traduire ces indices
    }
  }
  return squares;
}

game init_game(game g, uint* nb_tents_row, uint* nb_tents_col, square* squares) {
  for (uint i = 0; i < game_nb_rows(g);
       i++) {  // Initialisation du tableau avec le nombre de tentes attendu pour chaque ligne
    game_set_expected_nb_tents_row(g, i, nb_tents_row[i]);
  }
  for (uint j = 0; j < game_nb_cols(g);
       j++) {  // Initialisation du tableau avec le nombre de tentes attendu pour chaque colonne
    game_set_expected_nb_tents_col(g, j, nb_tents_col[j]);
  }
  g->board = square_to_board_aux(g, squares, game_nb_rows(g), game_nb_cols(g));
  return g;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint nb_random(uint x) { return (((double)rand() * x) / RAND_MAX); }

square tree_random() {  // renvoie aléatoirement TREE ou BIGTREE
  uint choix = rand() % 2;
  if (choix == 1) {
    return BIGTREE;
  }
  return TREE;
}

direction opp_simple(direction d) {  // renvoie la direction opposée à d (uniquement pour UP, DOWN, LEFT, RIGHT)
  if (d <= HERE || d > RIGHT) {
    exit(EXIT_FAILURE);
  }
  if (d == UP) return DOWN;
  if (d == DOWN) return UP;
  if (d == RIGHT) return LEFT;
  if (d == LEFT) return RIGHT;
  return HERE;
}
