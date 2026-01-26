#define __USE_GNU
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

/* ********** TEST ********** */

bool test_game_save() {
  game g = game_new_empty();  // On crée un jeu par défaut
  if (g == NULL) return false;

  // On modifie quelques cases et valeurs attendues pour vérifier que le save fonctionne quand on relira le fichier
  game_set_square(g, 0, 0, TREE);
  game_set_square(g, 1, 1, EMPTY);
  game_set_square(g, 2, 2, GRASS);
  game_set_square(g, 3, 3, TENT);
  game_set_square(g, 4, 4, BIGTREE);
  game_set_expected_nb_tents_row(g, 0, 1);
  game_set_expected_nb_tents_col(g, 0, 2);

  game_save(g, "levels/level_test.txt");  // On sauvegarde le jeu dans un fichier
  game_delete(g);
  return true;
}

bool test_game_load() {
  game g = game_load("levels/level_test.txt");
  if (g == NULL) return false;

  // Vérification de quelques cases pour s'assurer que le jeu est bien chargé
  if (game_get_square(g, 0, 0) != TREE) {
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 1, 1) != EMPTY) {
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 2, 2) != GRASS) {
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 3, 3) != TENT) {
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 4, 4) != BIGTREE) {
    game_delete(g);
    return false;
  }
  if (game_get_expected_nb_tents_row(g, 0) != 1) {
    game_delete(g);
    return false;
  }
  if (game_get_expected_nb_tents_col(g, 0) != 2) {
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
  if (strcmp("game_save", argv[1]) == 0)
    ok = test_game_save();
  else if (strcmp("game_load", argv[1]) == 0)
    ok = test_game_load();
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