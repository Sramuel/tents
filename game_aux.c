#include "game_aux.h"

#include <stdio.h>

#include "game_struct.h"

void game_print(cgame g) {
  uint i;
  uint j;
  printf("   ");
  for (j = 0; j < g->nb_cols; j++) {
    printf("%u", j);  // print de 0 à nb colonnes -1
  }
  printf("\n   ");
  for (j = 0; j < g->nb_cols; j++) {
    printf("-");  // --------
  }
  printf("\n");
  for (i = 0; i < g->nb_rows; i++) {  // print du plateau
    printf("%u |", i);                // print de 0 à nb lignes -1
    for (j = 0; j < g->nb_cols; j++) {
      if (g->board[i][j] == TREE) {
        printf("x");
      }
      if (g->board[i][j] == EMPTY) {
        printf(" ");
      }
      if (g->board[i][j] == GRASS) {
        printf("-");
      }
      if (g->board[i][j] == TENT) {
        printf("*");
      }
    }
    printf("| %u\n", g->nb_tents_row[i]);  // print des expected tentes dans les lignes
  }
  printf("   ");
  for (j = 0; j < g->nb_cols; j++) {
    printf("-");  // --------
  }
  printf("\n   ");
  for (j = 0; j < g->nb_cols; j++) {
    printf("%u", g->nb_tents_col[j]);  // print des expected tentes dans les colonnes
  }
  printf("\n");
}

game game_default(void) {
  game g = game_new_empty();

  g->nb_tents_row[0] = 1;
  g->nb_tents_row[1] = 2;
  g->nb_tents_row[2] = 1;
  g->nb_tents_row[3] = 2;
  g->nb_tents_row[4] = 2;
  g->nb_tents_row[5] = 0;
  g->nb_tents_row[6] = 1;
  g->nb_tents_row[7] = 3;

  g->nb_tents_col[0] = 3;
  g->nb_tents_col[1] = 0;
  g->nb_tents_col[2] = 2;
  g->nb_tents_col[3] = 1;
  g->nb_tents_col[4] = 2;
  g->nb_tents_col[5] = 1;
  g->nb_tents_col[6] = 2;
  g->nb_tents_col[7] = 1;

  g->board[0][0] = TREE;
  g->board[0][1] = EMPTY;
  g->board[0][2] = EMPTY;
  g->board[0][3] = EMPTY;
  g->board[0][4] = EMPTY;
  g->board[0][5] = TREE;
  g->board[0][6] = TREE;
  g->board[0][7] = EMPTY;

  g->board[1][0] = EMPTY;
  g->board[1][1] = EMPTY;
  g->board[1][2] = EMPTY;
  g->board[1][3] = EMPTY;
  g->board[1][4] = EMPTY;
  g->board[1][5] = EMPTY;
  g->board[1][6] = EMPTY;
  g->board[1][7] = EMPTY;

  g->board[2][0] = EMPTY;
  g->board[2][1] = EMPTY;
  g->board[2][2] = EMPTY;
  g->board[2][3] = EMPTY;
  g->board[2][4] = EMPTY;
  g->board[2][5] = EMPTY;
  g->board[2][6] = TREE;
  g->board[2][7] = EMPTY;

  g->board[3][0] = TREE;
  g->board[3][1] = EMPTY;
  g->board[3][2] = EMPTY;
  g->board[3][3] = TREE;
  g->board[3][4] = EMPTY;
  g->board[3][5] = EMPTY;
  g->board[3][6] = EMPTY;
  g->board[3][7] = EMPTY;

  g->board[4][0] = EMPTY;
  g->board[4][1] = EMPTY;
  g->board[4][2] = EMPTY;
  g->board[4][3] = EMPTY;
  g->board[4][4] = EMPTY;
  g->board[4][5] = TREE;
  g->board[4][6] = EMPTY;
  g->board[4][7] = EMPTY;

  g->board[5][0] = EMPTY;
  g->board[5][1] = EMPTY;
  g->board[5][2] = EMPTY;
  g->board[5][3] = TREE;
  g->board[5][4] = EMPTY;
  g->board[5][5] = EMPTY;
  g->board[5][6] = EMPTY;
  g->board[5][7] = EMPTY;

  g->board[6][0] = EMPTY;
  g->board[6][1] = EMPTY;
  g->board[6][2] = EMPTY;
  g->board[6][3] = EMPTY;
  g->board[6][4] = EMPTY;
  g->board[6][5] = EMPTY;
  g->board[6][6] = EMPTY;
  g->board[6][7] = EMPTY;

  g->board[7][0] = EMPTY;
  g->board[7][1] = TREE;
  g->board[7][2] = TREE;
  g->board[7][3] = EMPTY;
  g->board[7][4] = EMPTY;
  g->board[7][5] = TREE;
  g->board[7][6] = TREE;
  g->board[7][7] = EMPTY;

  return g;
}

game game_default_solution(void) { return NULL; }
bool game_get_ajacent_square(cgame g, uint i, uint j, direction d, uint* pi_next, uint* pj_next) { return false; }
bool game_is_neigh(cgame g, uint i, uint j, square s, bool diag) { return false; }