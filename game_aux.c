#include "game_aux.h"

#include <stdio.h>

#include "game_struct.h"

void game_print(cgame g) {
  int i;
  int j;
  printf("   ");
  for (j = 0; j < g->nb_cols; j++) {
    printf("%d", j);  // print de 0 à nb colonnes -1
  }
  printf("\n   ");
  for (j = 0; j < g->nb_cols; j++) {
    printf("-");  // --------
  }
  printf("\n");
  for (i = 0; i < g->nb_rows; i++) {  // print du plateau
    printf("%d |", i);                // print de 0 à nb lignes -1
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
    printf("| %d\n", g->nb_tents_row[i]);  // print des expected tentes dans les lignes
  }
  printf("   ");
  for (j = 0; j < g->nb_cols; j++) {
    printf("-");  // --------
  }
  printf("\n   ");
  for (j = 0; j < g->nb_cols; j++) {
    printf("%d", g->nb_tents_col[j]);  // print des expected tentes dans les colonnes
  }
  printf("\n");
}

game game_default(void) { return NULL; }
game game_default_solution(void) { return NULL; }
bool game_get_ajacent_square(cgame g, uint i, uint j, direction d, uint* pi_next, uint* pj_next) { return false; }
bool game_is_neigh(cgame g, uint i, uint j, square s, bool diag) { return false; }