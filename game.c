#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "game_struct.h"

game game_new(square* squares, uint* nb_tents_row, uint* nb_tents_col) { return NULL; }

game game_new_empty(void) {
  game g = malloc(sizeof(struct game_s));
  if (g == NULL) {
    return NULL;
  }
  g->board = malloc(DEFAULT_SIZE * sizeof(square*));  // allocation du premier tableau (première ligne)
  if (g->board == NULL) {
    game_delete(g);
    return NULL;
  }
  g->nb_rows = DEFAULT_SIZE;  // nombre de lignes
  g->nb_cols = DEFAULT_SIZE;  // nombre de colonnes

  g->nb_tents_row =
      calloc(DEFAULT_SIZE, sizeof(uint));  // allocation du tableau qui contient le nombre de tentes par ligne
  if (g->nb_tents_row == NULL) {
    game_delete(g);
    return NULL;
  }
  g->nb_tents_col =
      calloc(DEFAULT_SIZE, sizeof(uint));  // allocation du tableau qui contient le nombre de tentes par colonne
  if (g->nb_tents_col == NULL) {
    game_delete(g);
    return NULL;
  }
  for (uint i = 0; i < DEFAULT_SIZE;
       i++) {  // allocation de toutes les cases (une colonne pour chaque case de la première ligne)
    g->board[i] = calloc(DEFAULT_SIZE, sizeof(square));
    if (g->board[i] == NULL) {
      game_delete(g);
      return NULL;
    }
  }
  return g;
}

game game_copy(cgame g) { return NULL; }
bool game_equal(cgame g1, cgame g2) { return false; }
void game_delete(game g) {
  if (g) {
    for (uint i = 0; i < g->nb_rows; i++) {
      free(g->board[i]);
    }
    free(g->board);
    free(g);
  }
}

void game_set_square(game g, uint i, uint j, square s) {}
square game_get_square(cgame g, uint i, uint j) { return EMPTY; }
void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents) {}
void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents) {}
uint game_get_expected_nb_tents_row(cgame g, uint i) { return g->nb_tents_row[i]; }
uint game_get_expected_nb_tents_col(cgame g, uint j) { return g->nb_tents_col[j]; }
uint game_get_expected_nb_tents_all(cgame g) {
  uint somme = 0;
  for (uint i = 0; i < g->nb_rows; i++) {
    somme = somme + g->nb_tents_row[i];
  }
  return somme;
}
uint game_get_current_nb_tents_row(cgame g, uint i) {
  uint somme = 0;
  for (uint j = 0; j < g->nb_cols; j++) {
    if (g->board[i][j] == TENT) {
      somme++;
    }
  }
  return somme;
}
uint game_get_current_nb_tents_col(cgame g, uint j) {
  uint somme = 0;
  for (uint i = 0; i < g->nb_rows; i++) {
    if (g->board[i][j] == TENT) {
      somme++;
    }
  }
  return somme;
}
uint game_get_current_nb_tents_all(cgame g) {
  uint somme = 0;
  for (uint i = 0; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      if (g->board[i][j] == TENT) {
        somme++;
      }
    }
  }
  return somme;
}
void game_play_move(game g, uint i, uint j, square s) { g->board[i][j] = s; }
bool game_check_move(cgame g, uint i, uint j, square s) { return false; }
bool game_won(cgame g) { return false; }
void game_fill_grass(game g) {}
void game_restart(game g) {}