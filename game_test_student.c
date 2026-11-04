#define __USE_GNU
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

/* ********** TEST ********** */

bool test_dummy() { return true; }

bool test_print() {
  game g = game_new_empty();
  // On place une GRASS, trois TENT, un BIGTREE et un TREE pour le coverage
  game_set_square(g, 5, 5, GRASS);
  game_set_square(g, 0, 0, TREE);
  game_set_square(g, 7, 7, BIGTREE);
  game_set_square(g, 6, 7, TENT);
  game_set_square(g, 7, 6, TENT);
  game_set_square(g, 0, 1, TENT);
  game_print(g);
  game_delete(g);
  return true;
}

bool test_delete() {
  game g = game_default();
  game_delete(g);
  return true;
}

bool test_get_expected_nb_tents_row() {
  game g = game_default();
  if (g == NULL) return false;
  uint val[DEFAULT_SIZE] = {1, 2, 1, 2,
                            2, 0, 1, 3};  // Les valeurs par défaut des tents sur chaque ligne de la game_default
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_expected_nb_tents_row(g, i) != val[i]) {  // Vérification des valeurs
      game_delete(g);
      return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_get_expected_nb_tents_col() {
  game g = game_default();
  if (g == NULL) return false;
  uint val[DEFAULT_SIZE] = {3, 0, 2, 1,
                            2, 1, 2, 1};  // Les valeurs par défaut des tents sur chaque colonne de la game_default
  for (uint j = 0; j < DEFAULT_SIZE; j++) {
    if (game_get_expected_nb_tents_col(g, j) != val[j]) {  // Vérification des valeurs
      game_delete(g);
      return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_get_expected_nb_tents_all() {
  game g = game_default();
  if (g == NULL) return false;
  uint val = 0;
  for (uint i = 0; i < DEFAULT_SIZE; i++) {  // La somme des tentes attendues sur chaque ligne est la même que la somme
                                             // des tentes attendues sur une grille
    val += game_get_expected_nb_tents_row(g, i);
  }
  uint total = game_get_expected_nb_tents_all(g);  // Récupère le nombre total de tentes attendues
  game_delete(g);
  if (val == total) {  // Compare les deux valeurs
    return true;
  }
  return false;
}

bool test_get_current_nb_tents_row() {
  game g = game_default();
  if (g == NULL) return false;

  // On place des tentes
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
  game_set_square(g, 5, 1, TENT);

  // On place de l'herbe voir si elles ne sont pas comptabilisé comme des tentes
  game_set_square(g, 0, 1, GRASS);
  game_set_square(g, 1, 3, GRASS);
  game_set_square(g, 2, 5, GRASS);
  game_set_square(g, 3, 0, GRASS);
  game_set_square(g, 4, 6, GRASS);
  game_set_square(g, 6, 7, GRASS);
  game_set_square(g, 7, 4, GRASS);
  uint expected_row[8] = {1, 2, 1, 2, 2, 1, 1, 3};  // Valeurs attendues
  for (uint i = 0; i < DEFAULT_SIZE; i++) {         // On parcourt les lignes voir si les valeurs sont correctes
    if (game_get_current_nb_tents_row(g, i) != expected_row[i]) {
      game_delete(g);
      return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_get_current_nb_tents_col() {
  game g = game_default();
  if (g == NULL) return false;

  // On place des tentes
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
  game_set_square(g, 3, 1, TENT);

  // On place de l'herbe voir si elles ne sont pas comptabilisé comme des tentes
  game_set_square(g, 0, 2, GRASS);
  game_set_square(g, 1, 4, GRASS);
  game_set_square(g, 2, 7, GRASS);
  game_set_square(g, 3, 5, GRASS);
  game_set_square(g, 4, 3, GRASS);
  game_set_square(g, 6, 0, GRASS);
  game_set_square(g, 7, 6, GRASS);
  uint expected_col[8] = {3, 1, 2, 1, 2, 1, 2, 1};  // Valeurs attendues
  for (uint j = 0; j < DEFAULT_SIZE; j++) {         // On parcourt les colonnes voir si les valeurs sont correctes
    if (game_get_current_nb_tents_col(g, j) != expected_col[j]) {
      game_delete(g);
      return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_get_current_nb_tents_all() {
  game g = game_default();
  if (g == NULL) return false;

  // On va vérifier le nombre de tentes au fur et à mesure qu'on en place ou qu'on en enlève
  if (game_get_current_nb_tents_all(g) != 0) {
    game_delete(g);
    return false;
  }
  game_play_move(g, 4, 3, TENT);
  if (game_get_current_nb_tents_all(g) != 1) {
    game_delete(g);
    return false;
  }
  game_play_move(g, 0, 1, TENT);
  if (game_get_current_nb_tents_all(g) != 2) {
    game_delete(g);
    return false;
  }
  game_play_move(g, 0, 1, EMPTY);
  if (game_get_current_nb_tents_all(g) != 1) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_play_move() {
  game g = game_default();
  if (g == NULL) return false;

  // On va vérifier que les coups sont bien joués, donc on teste pour chaque type de square jouable
  game_play_move(g, 4, 3, TENT);
  if (game_get_square(g, 4, 3) != TENT) {
    game_delete(g);
    return false;
  }
  game_play_move(g, 4, 3, EMPTY);
  if (game_get_square(g, 4, 3) != EMPTY) {
    game_delete(g);
    return false;
  }
  game_play_move(g, 0, 1, GRASS);
  if (game_get_square(g, 0, 1) != GRASS) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_undo() {
  game g = game_new_empty();
  if (g == NULL) return false;
  // On joue deux coups pour pouvoir les annuler voir si le undo s'applique correctement
  game_play_move(g, 4, 3, TENT);
  game_play_move(g, 0, 1, GRASS);
  game_undo(g);
  if (game_get_square(g, 0, 1) != EMPTY) {  // Vérifie que le deuxième coup est bien annulé
    game_delete(g);
    return false;
  }
  game_undo(g);
  if (game_get_square(g, 4, 3) != EMPTY) {  // Vérifie que le premier coup est bien annulé
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_redo() {
  game g = game_new_empty();
  if (g == NULL) return false;
  // On joue deux coups pour pouvoir les annuler puis les refaire voir si le redo s'applique correctement
  game_play_move(g, 4, 3, TENT);
  game_play_move(g, 0, 1, GRASS);
  game_undo(g);
  game_undo(g);
  game_redo(g);
  if (game_get_square(g, 4, 3) != TENT) {  // Vérifie que le premier coup est bien refait
    game_delete(g);
    return false;
  }
  game_redo(g);
  if (game_get_square(g, 0, 1) != GRASS) {  // Vérifie que le deuxième coup est bien refait
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[]) {
  if (argc == 1) usage(argc, argv);

  // start test
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy", argv[1]) == 0)
    ok = test_dummy();
  else if (strcmp("print", argv[1]) == 0)
    ok = test_print();
  else if (strcmp("delete", argv[1]) == 0)
    ok = test_delete();
  else if (strcmp("get_expected_nb_tents_row", argv[1]) == 0)
    ok = test_get_expected_nb_tents_row();
  else if (strcmp("get_expected_nb_tents_col", argv[1]) == 0)
    ok = test_get_expected_nb_tents_col();
  else if (strcmp("get_expected_nb_tents_all", argv[1]) == 0)
    ok = test_get_expected_nb_tents_all();
  else if (strcmp("get_current_nb_tents_col", argv[1]) == 0)
    ok = test_get_current_nb_tents_col();
  else if (strcmp("get_current_nb_tents_row", argv[1]) == 0)
    ok = test_get_current_nb_tents_row();
  else if (strcmp("get_current_nb_tents_all", argv[1]) == 0)
    ok = test_get_current_nb_tents_all();
  else if (strcmp("play_move", argv[1]) == 0)
    ok = test_play_move();
  else if (strcmp("game_undo", argv[1]) == 0)
    ok = test_game_undo();
  else if (strcmp("game_redo", argv[1]) == 0)
    ok = test_game_redo();
  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // print test result
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
