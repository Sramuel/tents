#include <stdio.h>
#include <stdlib.h>

#include "game_aux.h"
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
  free(squares);
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
    for (uint i = 0; i < g->nb_rows; i++) {  // Libération de chaque ligne du tableau
      free(g->board[i]);
    }
    free(g->board);         // Libération du tableau de pointeurs vers les lignes
    free(g->nb_tents_col);  // Libération du tableau contenant le nombre de tentes par colonne
    free(g->nb_tents_row);  // Libération du tableau contenant le nombre de tentes par ligne
    free(g);                // Libération de la structure game
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

uint game_get_expected_nb_tents_row(cgame g, uint i) {
  return g->nb_tents_row[i];
}  // retourne le nombre de tentes attendu sur la ligne
uint game_get_expected_nb_tents_col(cgame g, uint j) {
  return g->nb_tents_col[j];
}  // retourne le nombre de tentes attendu sur la colonne
uint game_get_expected_nb_tents_all(cgame g) {
  uint somme = 0;
  for (uint i = 0; i < g->nb_rows; i++) {  // somme du nombre de tentes attendu pour chaque ligne
    somme = somme + g->nb_tents_row[i];
  }
  return somme;
}
uint game_get_current_nb_tents_row(cgame g, uint i) {
  uint somme = 0;
  for (uint j = 0; j < g->nb_cols; j++) {  // parcours de la ligne i
    if (g->board[i][j] == TENT) {
      somme++;
    }
  }
  return somme;
}
uint game_get_current_nb_tents_col(cgame g, uint j) {
  uint somme = 0;
  for (uint i = 0; i < g->nb_rows; i++) {  // parcours de la colonne j
    if (g->board[i][j] == TENT) {
      somme++;
    }
  }
  return somme;
}
uint game_get_current_nb_tents_all(cgame g) {
  uint somme = 0;
  for (uint i = 0; i < g->nb_rows; i++) {  // parcours de toutes les cases
    for (uint j = 0; j < g->nb_cols; j++) {
      if (g->board[i][j] == TENT) {
        somme++;  // compte le nombre de tentes
      }
    }
  }
  return somme;
}
void game_play_move(game g, uint i, uint j, square s) {
  cell* last_action = malloc(sizeof(cell)); // allocation de la cellule pour stocker l'action
  if (last_action == NULL) {
    return;
  }
  last_action->row = i;
  last_action->col = j;
  last_action->value = g->board[i][j];
  queue_push_head(g->undo, last_action); // sauvegarde de l'action pour undo
  g->board[i][j] = s; // joue le coup en mettant la case i,j à s
  if (g->redo != NULL) { // vide la pile redo
    queue_clear_full(g->redo, free); // libère chaque cellule de la pile redo
  }
}

bool game_check_move(cgame g, uint i, uint j, square s) {
  if (g == NULL) {  // verifie que g n'est pas NULL
    return false;
  }
  if (i >= g->nb_rows || j >= g->nb_cols) {  // verifie que i et j sont bien dans la grille
    return false;
  }
  if (s < 0 || s > 3) {  // verifie la validité de la case (EMPTY, GRASS, TENT or TREE)
    return false;
  }
  if (s == TREE) {  // on ne peux pas poser un nouvel arbre
    return false;
  }
  if (g->board[i][j] == TREE) {  // on ne peux remplacer un arbre par une autre case
    return false;
  }
  return true;
}

bool game_won(cgame g) {
  uint nb_trees = 0;
  for (uint i = 0; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      if (game_get_square(g, i, j) == TREE) {
        nb_trees++;                                  // Compteur d'arbre
        if (!game_is_neigh(g, i, j, TENT, false)) {  // Regle 4)
          return false;
        }
      }
    }
  }
  if (game_get_current_nb_tents_all(g) != nb_trees) {  // Regle 3)
    return false;
  }
  for (uint i = 0; i < g->nb_rows; i++) {  // Regle 2)
    if (game_get_current_nb_tents_row(g, i) != game_get_expected_nb_tents_row(g, i)) {
      return false;
    }
  }
  for (uint j = 0; j < g->nb_cols; j++) {  // Regle 2)
    if (game_get_current_nb_tents_col(g, j) != game_get_expected_nb_tents_col(g, j)) {
      return false;
    }
  }
  for (uint i = 0; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      if (game_get_square(g, i, j) == TENT) {
        if (!game_is_neigh(g, i, j, TREE, false)) {  // Regle 4) deuxieme verification
          return false;
        }
        if (game_is_neigh(g, i, j, TENT, true)) {  // Regle 1)
          return false;
        }
      }
    }
  }
  return true;
}

void game_fill_grass(game g) {
  if (g == NULL) {
    return;
  }
  for (int i = 0; i < g->nb_rows; i++) {
    for (int j = 0; j < g->nb_cols; j++) {
      if (g->board[i][j] == EMPTY) {
        game_set_square(g, i, j, GRASS);  // transforme toutes les cases EMPTY en case GRASS
      }
    }
  }
  return;
}
void game_restart(game g) {
  if (g == NULL) {
    return;
  }
  for (int i = 0; i < g->nb_rows; i++) {
    for (int j = 0; j < g->nb_cols; j++) {
      if (g->board[i][j] != TREE) {
        game_set_square(g, i, j, EMPTY);  // transforme toutes les cases non TREE en case EMPTY
      }
    }
  }
  return;
}