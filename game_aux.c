#include "game_aux.h"

#include <stdio.h>

#include "game.h"
#include "game_ext.h"
#include "game_struct.h"

void game_print(cgame g) {
  uint i;
  uint j;
  printf("   ");
  for (j = 0; j < game_nb_cols(g); j++) {
    printf("%u", j);  // print de 0 à nb colonnes -1
  }
  printf("\n   ");
  for (j = 0; j < game_nb_cols(g); j++) {
    printf("-");  // --------
  }
  printf("\n");
  for (i = 0; i < game_nb_rows(g); i++) {  // print du plateau
    printf("%u |", i);                     // print de 0 à nb lignes -1
    for (j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == TREE) {
        printf("x");
      }
      if (game_get_square(g, i, j) == EMPTY) {
        printf(" ");
      }
      if (game_get_square(g, i, j) == GRASS) {
        printf("-");
      }
      if (game_get_square(g, i, j) == TENT) {
        printf("*");
      }
      if (game_get_square(g, i, j) == BIGTREE) {
        printf("#");
      }
    }
    printf("| %u\n", game_get_expected_nb_tents_row(g, i));  // print des expected tentes dans les lignes
  }
  printf("   ");
  for (j = 0; j < game_nb_cols(g); j++) {
    printf("-");  // --------
  }
  printf("\n   ");
  for (j = 0; j < game_nb_cols(g); j++) {
    printf("%u", game_get_expected_nb_tents_col(g, j));  // print des expected tentes dans les colonnes
  }
  printf("\n");
}

game game_default(void) {
  game g = game_new_empty();
  if (g == NULL) return NULL;

  // Configuration du nombre de tentes attendues par ligne
  game_set_expected_nb_tents_row(g, 0, 1);
  game_set_expected_nb_tents_row(g, 1, 2);
  game_set_expected_nb_tents_row(g, 2, 1);
  game_set_expected_nb_tents_row(g, 3, 2);
  game_set_expected_nb_tents_row(g, 4, 2);
  game_set_expected_nb_tents_row(g, 5, 0);
  game_set_expected_nb_tents_row(g, 6, 1);
  game_set_expected_nb_tents_row(g, 7, 3);

  // Configuration du nombre de tentes attendues par colonne
  game_set_expected_nb_tents_col(g, 0, 3);
  game_set_expected_nb_tents_col(g, 1, 0);
  game_set_expected_nb_tents_col(g, 2, 2);
  game_set_expected_nb_tents_col(g, 3, 1);
  game_set_expected_nb_tents_col(g, 4, 2);
  game_set_expected_nb_tents_col(g, 5, 1);
  game_set_expected_nb_tents_col(g, 6, 2);
  game_set_expected_nb_tents_col(g, 7, 1);

  // Remplissage de la grille, on met uniquement les arbres

  // Ligne 0
  game_set_square(g, 0, 0, TREE);
  game_set_square(g, 0, 5, TREE);
  game_set_square(g, 0, 6, TREE);

  // Ligne 2
  game_set_square(g, 2, 6, TREE);

  // Ligne 3
  game_set_square(g, 3, 0, TREE);
  game_set_square(g, 3, 2, TREE);

  // Ligne 4
  game_set_square(g, 4, 4, TREE);

  // Ligne 5
  game_set_square(g, 5, 2, TREE);

  // Ligne 7
  game_set_square(g, 7, 1, TREE);
  game_set_square(g, 7, 2, TREE);
  game_set_square(g, 7, 5, TREE);
  game_set_square(g, 7, 6, TREE);

  return g;
}

game game_default_solution(void) {
  game g = game_default();
  if (g == NULL) return NULL;

  // Placement des tentes correspondant à la solution complète du jeu par défaut
  game_set_square(g, 0, 4, TENT);
  game_set_square(g, 1, 0, TENT);
  game_set_square(g, 1, 6, TENT);
  game_set_square(g, 2, 2, TENT);
  game_set_square(g, 3, 4, TENT);
  game_set_square(g, 3, 6, TENT);
  game_set_square(g, 4, 0, TENT);
  game_set_square(g, 4, 2, TENT);
  game_set_square(g, 6, 5, TENT);
  game_set_square(g, 7, 0, TENT);
  game_set_square(g, 7, 3, TENT);
  game_set_square(g, 7, 7, TENT);

  game_fill_grass(g);

  return g;
}

bool game_get_adjacent_square(cgame g, uint i, uint j, direction d, uint* pi_next, uint* pj_next) {
  if (g == NULL) {  // Vérifie que g n'est pas NULL
    return false;
  }
  if (i >= game_nb_rows(g) || j >= game_nb_cols(g)) {  // Vérifie que i et j sont bien dans la grille
    return false;
  }
  if (d < HERE || d > DOWN_RIGHT) {  // Vérifie que la direction d est valide (compris entre HERE et DOWN_RIGHT)
    return false;
  }
  const int dj[] = {0, 0, 0, -1, 1, -1, 1, -1, 1};  // tableau des abscisses des directions
  const int di[] = {0, -1, 1, 0, 0, -1, -1, 1, 1};  // tableau des ordonnées des directions
  int ni = i + di[d];                               // abscisse de la case adjacente
  int nj = j + dj[d];                               // ordonnée de la case adjacente

  if (game_is_wrapping(g)) {  // implementation de l'option wrapping
    if (ni < 0)
      ni = game_nb_rows(g) - 1;
    else if (ni >= game_nb_rows(g))
      ni = 0;

    if (nj < 0)
      nj = game_nb_cols(g) - 1;
    else if (nj >= game_nb_cols(g))
      nj = 0;
  }

  if (ni < 0 || nj < 0 || ni >= game_nb_rows(g) || nj >= game_nb_cols(g)) {
    return false;
  }
  *pi_next = (uint)ni;
  *pj_next = (uint)nj;

  return true;
}

bool game_is_neigh(cgame g, uint i, uint j, square s, bool diag) {
  if (g == NULL) {  // Vérifie que g n'est pas NULL
    return false;
  }
  if (i >= game_nb_rows(g) || j >= game_nb_cols(g)) {  // Vérifie que i et j sont bien dans la grille
    return false;
  }

  return (game_nb_neigh(g, i, j, s, diag) > 0);
}

uint game_nb_neigh(cgame g, uint i, uint j, square s, bool diag) {
  if (g == NULL) {  // Vérifie que g n'est pas NULL
    return 0;
  }
  if (i >= game_nb_rows(g) || j >= game_nb_cols(g)) {  // Vérifie que i et j sont bien dans la grille
    return 0;
  }

  uint neigh_count = 0;

  if (diag) {
    const int di[] = {-1, 1, 0, 0, -1, -1, 1, 1};  // tableau des ordonnées des directions avec les diagonales
    const int dj[] = {0, 0, -1, 1, -1, 1, -1, 1};  // tableau des abscisses des directions avec les diagonales
    for (int k = 0; k < 8; k++) {
      int ni = i + di[k];  // ordonée de la case adjacente
      int nj = j + dj[k];  // abscisse de la case adjacente

      if (game_is_wrapping(g)) {  // implementation de l'option wrapping avec les diag
        if (ni < 0)
          ni = game_nb_rows(g) - 1;
        else if (ni >= game_nb_rows(g))
          ni = 0;

        if (nj < 0)
          nj = game_nb_cols(g) - 1;
        else if (nj >= game_nb_cols(g))
          nj = 0;
      }
      if (ni >= 0 && ni < game_nb_rows(g) && nj >= 0 && nj < game_nb_cols(g)) {
        if (game_get_square(g, ni, nj) == s) {
          neigh_count++;
        } else if (s == TREE && game_get_square(g, ni, nj) == BIGTREE) {  // Compte BIGTREE comme un arbre
          neigh_count++;
        }
      }
    }
  } else {
    const int di[] = {0, 0, -1, 1};  // tableau des ordonnées des directions sans les diagonales
    const int dj[] = {-1, 1, 0, 0};  // tableau des abscisses des directions sans les diagonales
    for (int k = 0; k < 4; k++) {
      int ni = i + di[k];  // ordonée de la case adjacente
      int nj = j + dj[k];  // abscisse de la case adjacente

      if (game_is_wrapping(g)) {  // implementation de l'option wrapping sans les diag
        if (ni < 0)
          ni = game_nb_rows(g) - 1;
        else if (ni >= game_nb_rows(g))
          ni = 0;

        if (nj < 0)
          nj = game_nb_cols(g) - 1;
        else if (nj >= game_nb_cols(g))
          nj = 0;
      }
      if (ni >= 0 && ni < game_nb_rows(g) && nj >= 0 && nj < game_nb_cols(g)) {
        if (game_get_square(g, ni, nj) == s) {
          neigh_count++;
        } else if (s == TREE && game_get_square(g, ni, nj) == BIGTREE) {  // Compte BIGTREE comme un arbre
          neigh_count++;
        }
      }
    }
  }
  return neigh_count;
}