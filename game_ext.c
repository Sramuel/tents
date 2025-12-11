#include "game_ext.h"

#include <stdio.h>
#include <stdlib.h>

#include "game_aux.h"
#include "game_struct.h"

game game_new_ext(uint nb_rows, uint nb_cols, square* squares, uint* nb_tents_row, uint* nb_tents_col, bool wrapping) {
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping);
  g = init_game(g, nb_tents_row, nb_tents_col, squares);
  return g;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping) { 
  game g = malloc(sizeof(struct game_s));
  if (g == NULL) {
    return NULL;
  }
  g->board = malloc(nb_cols * sizeof(square*));  // allocation du premier tableau (première ligne)
  if (g->board == NULL) {
    game_delete(g);
    return NULL;
  }
  g->nb_rows = nb_rows;  // nombre de lignes
  g->nb_cols = nb_cols;  // nombre de colonnes

  g->nb_tents_row =
      calloc(nb_rows, sizeof(uint));  // allocation du tableau qui contient le nombre de tentes par ligne
  if (g->nb_tents_row == NULL) {
    game_delete(g);
    return NULL;
  }
  g->nb_tents_col =
      calloc(nb_cols, sizeof(uint));  // allocation du tableau qui contient le nombre de tentes par colonne
  if (g->nb_tents_col == NULL) {
    game_delete(g);
    return NULL;
  }
  for (uint i = 0; i < nb_cols;
       i++) {  // allocation de toutes les cases (une colonne pour chaque case de la première ligne)
    g->board[i] = calloc(nb_rows, sizeof(square));
    if (g->board[i] == NULL) {
      game_delete(g);
      return NULL;
    }
  }
  g->wrapping = wrapping;
  g->undo = malloc(sizeof(queue*));
  if (g->undo == NULL){
      game_delete(g);
      return NULL;
    }
  g->redo = malloc(sizeof(queue*));
  if (g->redo == NULL){
      game_delete(g);
      return NULL;
    }
  return g;
}

uint game_nb_rows(cgame g) {
  if (g == NULL) {  // verifie que g n'est pas NULL
    return 0;
  }
  return g->nb_rows;
}

uint game_nb_cols(cgame g) {
  if (g == NULL) {  // verifie que g n'est pas NULL
    return 0;
  }
  return g->nb_cols;
}

bool game_is_wrapping(cgame g) {
  if (g == NULL) {  // verifie que g n'est pas NULL
    return false;
  }
  return g->wrapping;
}

void game_undo(game g) {
  if(g == NULL || queue_is_empty(g->undo)) {
    return;
  }
  cell* undo_action =  queue_pop_head(g->undo); // récupération de la dernière action
  if (undo_action == NULL) {
    return;
  }
  cell* redo_action =  malloc(sizeof(cell)); // allocation de la cellule pour stocker l'action à refaire
  if (redo_action == NULL) {
    queue_push_head(g->undo, undo_action); // remet l'action dans la pile undo en cas d'erreur
    return;
  }
  redo_action->value = g->board[undo_action->row][undo_action->col]; // valeur avant undo
  redo_action->row = undo_action->row;
  redo_action->col = undo_action->col;
  g->board[undo_action->row][undo_action->col] = undo_action->value; // restauration de la valeur précédente
  queue_push_head(g->redo, redo_action); // sauvegarde de l'action pour redo
  free(undo_action);
}

void game_redo(game g) {
  if(g == NULL || queue_is_empty(g->redo)) {
    return;
  }
  cell* redo_action =  queue_pop_head(g->redo); // récupération de la dernière action
  if (redo_action == NULL) {
    return;
  }
  cell* undo_action =  malloc(sizeof(cell)); // allocation de la cellule pour stocker l'action à annuler
  if (undo_action == NULL) {
    queue_push_head(g->redo, redo_action); // remet l'action dans la pile redo en cas d'erreur
    return;
  }
  undo_action->value = g->board[redo_action->row][redo_action->col]; // valeur avant redo
  undo_action->row = redo_action->row;
  undo_action->col = redo_action->col;
  g->board[redo_action->row][redo_action->col] = redo_action->value; // restauration de la valeur précédente
  queue_push_head(g->undo, undo_action); // sauvegarde de l'action pour undo
  free(redo_action);
}