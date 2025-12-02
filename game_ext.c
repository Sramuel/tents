#include "game_ext.h"

#include <stdio.h>
#include <stdlib.h>

#include "game_aux.h"
#include "game_struct.h"

game game_new_ext(uint nb_rows, uint nb_cols, square* squares, uint* nb_tents_row, uint* nb_tents_col, bool wrapping) {
  return NULL;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping) { return NULL; }

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