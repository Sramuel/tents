#include "game.h"

#include <stdio.h>
#include <stdlib.h>

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

square* board_to_square_aux (cgame g, uint nb_rows, uint nb_cols){
  square* squares = malloc(nb_cols*nb_rows*sizeof(square));
  if (squares == NULL){
    exit(EXIT_FAILURE);
  }
  for (uint i = 0; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      squares[i * nb_cols + j] = g->board[i][j];  // Squares est un simple tableau, il faut alors traduire ces indices
    }
  }
  return squares;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

game game_new(square* squares, uint* nb_tents_row, uint* nb_tents_col) {
  game g = game_new_empty();
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

game game_copy(cgame g) {
  square* squares = board_to_square_aux(g, g->nb_rows, g->nb_cols);  // Traduction du Double tableau en Simple tableau
  game g_copy = game_new(squares, g->nb_tents_row,
                         g->nb_tents_col);  // Création d'une game avec un Simple tableau et modifiée en Double tableau
  return g_copy;
}

bool game_equal(cgame g1, cgame g2) {
  if (g1 == NULL || g2 == NULL) {  // Vérification des pointeurs
    return false;
  }
  if (g1->nb_rows != g2->nb_rows) {  // Vérification du nombre de lignes
    return false;
  }
  if (g1->nb_cols != g2->nb_cols) {  // Vérification du nombre de colonnes
    return false;
  }
  for (uint i = 0; i < g1->nb_rows;
       i++) {  // Vérification du tableau contenant le nombre de tente attendu pour une ligne
    if (g1->nb_tents_row[i] != g2->nb_tents_row[i]) {
      return false;
    }
  }
  for (uint i = 0; i < g1->nb_cols;
       i++) {  // Vérification du tableau contenant le nombre de tente attendu pour une colonne
    if (g1->nb_tents_col[i] != g2->nb_tents_col[i]) {
      return false;
    }
  }
  for (uint i = 0; i < g1->nb_rows; i++) {  // Vérification du tableau contenant toutes les cases du jeu
    for (uint j = 0; j < g1->nb_cols; j++) {
      if (g1->board[i][j] != g2->board[i][j]) {
        return false;
      }
    }
  }
  return true;
}

void game_delete(game g) {
  if (g) {
    for (uint i = 0; i < g->nb_rows; i++) {
      free(g->board[i]);
    }
    free(g->board);
    free(g);
  }
}

void game_set_square(game g, uint i, uint j, square s) {
  if (g == NULL || i >= g->nb_rows || j >= g->nb_cols || s < 0 || s > 3) {  // Vérification des paramètres
    return;
  }
  g->board[i][j] = s;  // Mise à jour de la case
}

square game_get_square(cgame g, uint i, uint j) {
  if (g == NULL || i >= g->nb_rows || j >= g->nb_cols) {  // Vérification des paramètres
    exit(EXIT_FAILURE);
  }
  return g->board[i][j];  // Récupération de la valeur contenue dans la case
}

void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents) {
  if (g == NULL || i >= g->nb_rows) {
    return;
  }
  g->nb_tents_row[i] = nb_tents;  // Met le nombre de tentes attendu sur la ligne
}

void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents) {
  if (g == NULL || j >= g->nb_cols) {
    return;
  }
  g->nb_tents_col[j] = nb_tents;  // Met le nombre de tentes attendu sur la colonne
}

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