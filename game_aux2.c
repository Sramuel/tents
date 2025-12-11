#include "game_aux2.h"

#include <stdio.h>
#include <stdlib.h>

#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"

//////////////////////////////////////////////////////////////////////////AUXILIAIRES/////////////////////////////////////////////////////////////////////////////

square** square_to_board_aux(game g, square* squares, uint nb_rows, uint nb_cols) {
  for (uint i = 0; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      g->board[i][j] = squares[i * nb_cols + j];  // Squares est un simple tableau, il faut alors traduire ces indices
    }
  }
  return g->board;
}

square* board_to_square_aux(cgame g, uint nb_rows, uint nb_cols) {
  square* squares = malloc(nb_cols * nb_rows * sizeof(square));
  if (squares == NULL) {
    exit(EXIT_FAILURE);
  }
  for (uint i = 0; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      squares[i * nb_cols + j] = g->board[i][j];  // Squares est un simple tableau, il faut alors traduire ces indices
    }
  }
  return squares;
}

game init_game(game g, uint* nb_tents_row, uint* nb_tents_col, square* squares) {
  for (uint i = 0; i < g->nb_rows;
       i++) {  // Initialisation du tableau avec le nombre de tentes attendu pour chaque ligne
    g->nb_tents_row[i] = nb_tents_row[i];
  }
  for (uint i = 0; i < g->nb_cols;
       i++) {  // Initialisation du tableau avec le nombre de tentes attendu pour chaque colonne
    g->nb_tents_col[i] = nb_tents_col[i];
  }
  g->board = square_to_board_aux(g, squares, g->nb_rows, g->nb_cols);
  return g;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
