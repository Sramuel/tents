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
  g->board[3][2] = TREE;
  g->board[3][3] = EMPTY;
  g->board[3][4] = EMPTY;
  g->board[3][5] = EMPTY;
  g->board[3][6] = EMPTY;
  g->board[3][7] = EMPTY;

  g->board[4][0] = EMPTY;
  g->board[4][1] = EMPTY;
  g->board[4][2] = EMPTY;
  g->board[4][3] = EMPTY;
  g->board[4][4] = TREE;
  g->board[4][5] = EMPTY;
  g->board[4][6] = EMPTY;
  g->board[4][7] = EMPTY;

  g->board[5][0] = EMPTY;
  g->board[5][1] = EMPTY;
  g->board[5][2] = TREE;
  g->board[5][3] = EMPTY;
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

game game_default_solution(void) {
  game g = game_default();

  g->board[0][4] = TENT;
  g->board[1][0] = TENT;
  g->board[1][6] = TENT;
  g->board[2][2] = TENT;
  g->board[3][4] = TENT;
  g->board[3][6] = TENT;
  g->board[4][0] = TENT;
  g->board[4][2] = TENT;
  g->board[6][5] = TENT;
  g->board[7][0] = TENT;
  g->board[7][3] = TENT;
  g->board[7][7] = TENT;

  game_fill_grass(g);

  return g;
}

bool game_get_adjacent_square(cgame g, uint i, uint j, direction d, uint* pi_next, uint* pj_next) {
  if (g == NULL) {  // verifie que g n'est pas NULL
    return false;
  }
  if (i >= g->nb_rows || j >= g->nb_cols || i < 0 || j < 0) {  // verifie que i et j sont bien dans la grille
    return false;
  }
  if (d < HERE || d > DOWN_RIGHT) {  // verifie que d est valide (compris entre HERE et DOWN_RIGHT)
    return false;
  }
  const int dj[] = {0, 0, 0, -1, 1, -1, 1, -1, 1};  // tableau des abscisses des directions
  const int di[] = {0, -1, 1, 0, 0, -1, -1, 1, 1};  // tableau des ordonnées des directions
  *pi_next = i + di[d];                             // abscisse de la case adjacente
  *pj_next = j + dj[d];                             // ordonnée de la case adjacente

  if (*pi_next >= g->nb_rows || *pj_next >= g->nb_cols || *pi_next < 0 || *pj_next < 0) {
    return false;
  }
  return true;
}

bool game_is_neigh(cgame g, uint i, uint j, square s, bool diag) {
  if (g == NULL) {  // verifie que g n'est pas NULL
    return false;
  }
  if (i >= g->nb_rows || j >= g->nb_cols) {  // verifie que i et j sont bien dans la grille
    return false;
  }

  return (game_nb_neigh(g, i, j, s, diag) > 0);
}

uint game_nb_neigh(cgame g, uint i, uint j, square s, bool diag) {
  if (g == NULL) {  // verifie que g n'est pas NULL
    return 0;
  }
  if (i >= g->nb_rows || j >= g->nb_cols) {  // verifie que i et j sont bien dans la grille
    return 0;
  }

  uint neigh_count = 0;

  if (diag) {
    const int di[] = {-1, 1, 0, 0, -1, -1, 1, 1};  // tableau des ordonnées des directions
    const int dj[] = {0, 0, -1, 1, -1, 1, -1, 1};  // tableau des abscisses des directions
    for (int k = 0; k < 8; k++) {
      int ni = i + di[k];
      int nj = j + dj[k];

      if (g->wrapping) {  // implementation de l'option wrapping
        if (ni < 0)
          ni = g->nb_rows - 1;
        else if (ni >= g->nb_rows)
          ni = 0;

        if (nj < 0)
          nj = g->nb_cols - 1;
        else if (nj >= g->nb_cols)
          nj = 0;
      }
      if (ni >= 0 && ni < g->nb_rows && nj >= 0 && nj < g->nb_cols) {
        if (g->board[ni][nj] == s) {
          neigh_count++;
        } else if (s == TREE && g->board[ni][nj] == BIGTREE) {  // Compte BIGTREE comme un arbre
          neigh_count++;
        }
      }
    }
  } else {
    const int di[] = {0, 0, -1, 1};
    const int dj[] = {-1, 1, 0, 0};
    for (int k = 0; k < 4; k++) {
      int ni = i + di[k];
      int nj = j + dj[k];

      if (g->wrapping) {  // implementation de l'option wrapping
        if (ni < 0)
          ni = g->nb_rows - 1;
        else if (ni >= g->nb_rows)
          ni = 0;

        if (nj < 0)
          nj = g->nb_cols - 1;
        else if (nj >= g->nb_cols)
          nj = 0;
      }
      if (ni >= 0 && ni < g->nb_rows && nj >= 0 && nj < g->nb_cols) {
        if (g->board[ni][nj] == s) {
          neigh_count++;
        } else if (s == TREE && g->board[ni][nj] == BIGTREE) {  // Compte BIGTREE comme un arbre
          neigh_count++;
        }
      }
    }
  }
  return neigh_count;
}