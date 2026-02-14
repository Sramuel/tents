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
  game_set_square(g, 1, 1, GRASS);

  game_save(g, "levels/tmp_test.txt");  // On sauvegarde le jeu dans un fichier

  game g2 = game_load("levels/tmp_test.txt");  // Puis on reload le jeu depuis le fichier
  if (g2 == NULL) {
    game_delete(g);
    return false;
  }

  bool ok = (game_equal(g, g2));  // On compare les deux jeux

  game_delete(g);
  game_delete(g2);
  return ok;
}

bool test_game_load() {
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

  game g2 = game_load("levels/level_test.txt");  // Puis on reload le jeu depuis le fichier
  if (g2 == NULL) {
    game_delete(g);
    return false;
  }

  if (game_nb_rows(g) != game_nb_rows(g2) || game_nb_cols(g) != game_nb_cols(g2)) {  // Vérification des dimensions
    game_delete(g);
    game_delete(g2);
    return false;
  }

  if (game_is_wrapping(g) != game_is_wrapping(g2)) {  // Vérification Wrapping
    game_delete(g);
    game_delete(g2);
    return false;
  }

  for (uint i = 0; i < game_nb_rows(g); i++) {  // Vérification de toutes les cases
    for (uint j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) != game_get_square(g2, i, j)) {
        game_delete(g);
        game_delete(g2);
        return false;
      }
    }
  }

  for (uint i = 0; i < game_nb_rows(g); i++) {  // Vérification des lignes
    if (game_get_expected_nb_tents_row(g, i) != game_get_expected_nb_tents_row(g2, i)) {
      game_delete(g);
      game_delete(g2);
      return false;
    }
  }

  for (uint j = 0; j < game_nb_cols(g); j++) {  // Vérification des colonnes
    if (game_get_expected_nb_tents_col(g, j) != game_get_expected_nb_tents_col(g2, j)) {
      game_delete(g);
      game_delete(g2);
      return false;
    }
  }

  game_delete(g);
  game_delete(g2);
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