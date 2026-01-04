#include <stdio.h>
#include <stdlib.h>

#include "game_aux2.h"
#include "game_ext.h"

game game_new(square* squares, uint* nb_tents_row, uint* nb_tents_col) {
  game g = game_new_empty();
  g = init_game(g, nb_tents_row, nb_tents_col, squares);
  return g;
}

game game_new_empty(void) {
  game g = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, false);
  return g;
}

game game_copy(cgame g) {
  if (g == NULL) {
    return NULL;
  }
  square* squares = board_to_square_aux(g, g->nb_rows, g->nb_cols);  // Traduction du Double tableau en Simple tableau
  game g_copy = game_new_ext(g->nb_rows, g->nb_cols, squares, g->nb_tents_row, g->nb_tents_col,
                             g->wrapping);  // Création d'une game avec toutes les propriétés
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
  if (g1->wrapping != g2->wrapping) {  // Vérification de l'option wrapping
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
    if (g->undo != NULL) {
      queue_clear_full(g->undo, free);  // Libération de la pile undo
      queue_free(g->undo);
    }
    if (g->redo != NULL) {
      queue_clear_full(g->redo, free);  // Libération de la pile redo
      queue_free(g->redo);
    }
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
  if (g == NULL || i >= g->nb_rows || j >= g->nb_cols || s < 0 || s > 4) {  // Vérification des paramètres
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
  if (g == NULL) return 0;
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
  cell* last_action = malloc(sizeof(cell));  // allocation de la cellule pour stocker l'action
  if (last_action == NULL) {
    return;
  }
  last_action->row = i;
  last_action->col = j;
  last_action->value = g->board[i][j];
  queue_push_head(g->undo, last_action);  // sauvegarde de l'action pour undo
  g->board[i][j] = s;                     // joue le coup en mettant la case i,j à s
  if (g->redo != NULL) {                  // vide la pile redo
    queue_clear_full(g->redo, free);      // libère chaque cellule de la pile redo
  }
}

bool game_check_move(cgame g, uint i, uint j, square s) {
  if (g == NULL) {  // verifie que g n'est pas NULL
    return false;
  }
  if (i >= g->nb_rows || j >= g->nb_cols) {  // verifie que i et j sont bien dans la grille
    return false;
  }
  if (s < 0 || s > 4) {  // verifie la validité de la case (EMPTY, GRASS, TENT, TREE or BIGTREE)
    return false;
  }
  if (s == TREE || s == BIGTREE) {  // on ne peux pas poser un nouvel arbre ou un gros arbre
    return false;
  }
  if (g->board[i][j] == TREE || g->board[i][j] == BIGTREE) {  // on ne peux remplacer un arbre par une autre case
    return false;
  }
  return true;
}

bool game_won(cgame g) {
  if (g == NULL) {
    return false;
  }
  uint nb_trees = 0;     // compte les TREE et les BIGTREE
  uint nb_bigtrees = 0;  // compte uniquement les BIGTREE
  for (uint i = 0; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      if (game_get_square(g, i, j) == TREE || game_get_square(g, i, j) == BIGTREE) {
        nb_trees++;
        if (game_get_square(g, i, j) == TREE) {
          if (game_nb_neigh(g, i, j, TENT, false) < 1) return false;
        } else {
          nb_bigtrees++;
          if (game_nb_neigh(g, i, j, TENT, false) != 2) return false;
        }
      }
    }
  }
  uint expected_tents = (nb_trees - nb_bigtrees) + 2 * nb_bigtrees;
  if (game_get_current_nb_tents_all(g) != expected_tents) {  // Regle 3)
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
        bool has_tree = game_is_neigh(g, i, j, TREE, false);
        bool has_bigtree = game_is_neigh(g, i, j, BIGTREE, false);

        if (!(has_tree || has_bigtree)) {  // Regle 4) deuxieme verification
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
      if (g->board[i][j] != TREE && g->board[i][j] != BIGTREE) {
        game_set_square(g, i, j, EMPTY);  // transforme toutes les cases non TREE/BIGTREE en case EMPTY
      }
    }
  }
  if (g->undo != NULL) {
    queue_clear_full(g->undo, free);  // Vide la pile undo
  }
  if (g->redo != NULL) {
    queue_clear_full(g->redo, free);  // Vide la pile redo
  }
  return;
}