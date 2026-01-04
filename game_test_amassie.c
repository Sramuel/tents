#define __USE_GNU
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

bool test_dummy() { return true; }

bool test_game_is_neigh() {
  game g = game_new_empty();

  game_set_square(g, 3, 3, TREE);
  game_set_square(g, 3, 2, TREE);
  game_set_square(g, 3, 4, TREE);
  game_set_square(g, 2, 3, TREE);
  game_set_square(g, 4, 3, TREE);
  game_set_square(g, 2, 2, TREE);
  game_set_square(g, 4, 4, TREE);

  bool test1 = game_is_neigh(g, 3, 3, TREE, false);

  bool test2 = !game_is_neigh(g, 0, 0, TREE, false);

  bool test3 = !game_is_neigh(g, 1, 1, TREE, false);
  bool test4 = game_is_neigh(g, 1, 1, TREE, true);

  game_set_square(g, 7, 7, GRASS);
  bool test5 = !game_is_neigh(g, 7, 6, TREE, false);
  game_delete(g);
  return test1 && test2 && test3 && test4 && test5;
}

bool test_game_nb_neigh() {
  game g = game_new_empty();

  game_set_square(g, 0, 0, TREE);
  game_set_square(g, 1, 0, TREE);
  game_set_square(g, 2, 0, TREE);
  game_set_square(g, 0, 1, TENT);
  game_set_square(g, 2, 1, TENT);
  game_set_square(g, 2, 2, GRASS);

  bool test1 = game_nb_neigh(g, 1, 1, TREE, true) == 3;
  bool test2 = game_nb_neigh(g, 1, 1, TREE, false) == 1;

  bool test3 = game_nb_neigh(g, 1, 1, TENT, true) == 2;

  bool test4 = game_nb_neigh(g, 1, 1, GRASS, true) == 1;

  game_set_square(g, 7, 7, GRASS);
  bool test5 = game_nb_neigh(g, 7, 6, TREE, false) == 0;
  bool test6 = game_nb_neigh(g, 1, 1, TREE, true) == 4;
  game_delete(g);
  return test1 && test2 && test3 && test4 && test5 && !test6;
}

bool test_game_get_adjacent_square() {
  /*test sans wrapping*/
  game g = game_new_empty();
  uint i_next;
  uint j_next;

  if (!game_get_adjacent_square(g, 3, 3, UP, &i_next, &j_next)) return false;
  if (i_next != 2 || j_next != 3) return false;

  if (!game_get_adjacent_square(g, 3, 3, LEFT, &i_next, &j_next)) return false;
  if (i_next != 3 || j_next != 2) return false;

  if (!game_get_adjacent_square(g, 3, 3, DOWN_RIGHT, &i_next, &j_next)) return false;
  if (i_next != 4 || j_next != 4) return false;

  if (!game_get_adjacent_square(g, 3, 3, DOWN, &i_next, &j_next)) return false;
  if (i_next != 4 || j_next != 3) return false;

  if (game_get_adjacent_square(g, 0, 0, UP, &i_next, &j_next)) return false;
  if (game_get_adjacent_square(g, 0, 0, LEFT, &i_next, &j_next)) return false;
  if (game_get_adjacent_square(g, 0, 0, UP_LEFT, &i_next, &j_next)) return false;
  if (game_get_adjacent_square(g, 0, 0, UP_RIGHT, &i_next, &j_next)) return false;
  if (game_get_adjacent_square(g, 7, 7, RIGHT, &i_next, &j_next)) return false;
  if (game_get_adjacent_square(g, 7, 7, DOWN_LEFT, &i_next, &j_next)) return false;
  if (game_get_adjacent_square(g, 7, 7, DOWN_RIGHT, &i_next, &j_next)) return false;
  if (game_get_adjacent_square(g, 7, 7, DOWN, &i_next, &j_next)) return false;

  game_delete(g);

  /*test avec wrapping*/
  game gw = game_new_empty_ext(8, 8, true);
  if (gw == NULL) return false;

  if (!game_get_adjacent_square(gw, 0, 0, UP, &i_next, &j_next)) return false;
  if (i_next != 7 || j_next != 0) return false;

  if (!game_get_adjacent_square(gw, 0, 0, LEFT, &i_next, &j_next)) return false;
  if (i_next != 0 || j_next != 7) return false;

  if (!game_get_adjacent_square(gw, 0, 0, UP_LEFT, &i_next, &j_next)) return false;
  if (i_next != 7 || j_next != 7) return false;

  if (!game_get_adjacent_square(gw, 7, 7, DOWN, &i_next, &j_next)) return false;
  if (i_next != 0 || j_next != 7) return false;

  if (!game_get_adjacent_square(gw, 7, 7, RIGHT, &i_next, &j_next)) return false;
  if (i_next != 7 || j_next != 0) return false;

  if (!game_get_adjacent_square(gw, 7, 0, DOWN_LEFT, &i_next, &j_next)) return false;
  if (i_next != 0 || j_next != 7) return false;

  game_delete(gw);
  return true;
}

bool test_game_default_solution() {
  game sol = game_default_solution();
  game def = game_default();

  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(sol, i, j) == EMPTY) {
        game_delete(sol);
        game_delete(def);
        return false;
      }
    }
  }
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(sol, i, j) == TENT) {
        if (game_is_neigh(sol, i, j, TENT, true)) {
          game_delete(sol);
          game_delete(def);
          return false;
        }
      }
    }
  }
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(def, i, j) == TREE) {
        if (!game_is_neigh(sol, i, j, TENT, false)) {
          game_delete(sol);
          game_delete(def);
          return false;
        }
      }
    }
  }
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    uint nb = 0;
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(sol, i, j) == TENT) {
        nb++;
      }
    }
    if (nb != game_get_expected_nb_tents_row(def, i)) {
      game_delete(sol);
      game_delete(def);
      return false;
    }
  }
  for (uint j = 0; j < DEFAULT_SIZE; j++) {
    uint nb = 0;
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
      if (game_get_square(sol, i, j) == TENT) {
        nb++;
      }
    }
    if (nb != game_get_expected_nb_tents_col(def, j)) {
      game_delete(sol);
      game_delete(def);
      return false;
    }
  }
  game_delete(sol);
  game_delete(def);
  return true;
}

bool test_game_default() {
  game def = game_default();
  game sol = game_default_solution();

  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(def, i, j) != TREE && game_get_square(def, i, j) != EMPTY) {
        game_delete(sol);
        game_delete(def);
        return false;
      }
    }
  }
  uint trees_def = 0;
  uint trees_sol = 0;

  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(def, i, j) == TREE) trees_def++;
      if (game_get_square(sol, i, j) == TREE) trees_sol++;
    }
  }
  if (trees_def != trees_sol) {
    game_delete(sol);
    game_delete(def);
    return false;
  }
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_expected_nb_tents_row(def, i) != game_get_expected_nb_tents_row(sol, i)) {
      game_delete(def);
      game_delete(sol);
      return false;
    }
  }
  for (uint j = 0; j < DEFAULT_SIZE; j++) {
    if (game_get_expected_nb_tents_col(def, j) != game_get_expected_nb_tents_col(sol, j)) {
      game_delete(def);
      game_delete(sol);
      return false;
    }
  }
  game_delete(def);
  game_delete(sol);
  return true;
}

bool test_game_restart() {
  game g_original = game_default();
  if (g_original == NULL) {
    return false;
  }
  game g_to_restart = game_copy(g_original);
  if (g_to_restart == NULL) {
    game_delete(g_original);
    return false;
  }
  game_play_move(g_to_restart, 0, 1, TENT);
  game_play_move(g_to_restart, 1, 1, GRASS);
  if (game_equal(g_original, g_to_restart)) {
    game_delete(g_original);
    game_delete(g_to_restart);
    return false;
  }
  game_restart(g_to_restart);
  bool test_passed = game_equal(g_original, g_to_restart);
  if (!test_passed) {
    game_delete(g_original);
    game_delete(g_to_restart);
    return test_passed;
  }
  game_delete(g_original);
  game_delete(g_to_restart);
  return test_passed;
}

bool test_game_fill_grass() {
  game g = game_new_empty();
  game_set_square(g, 0, 0, TREE);
  game_set_square(g, 1, 1, TENT);
  game_set_square(g, 2, 2, GRASS);
  game_fill_grass(g);

  if (game_get_square(g, 0, 0) != TREE) {
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 1, 1) != TENT) {
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 2, 2) != GRASS) {
    game_delete(g);
    return false;
  }
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (!((i == 0 && j == 0) || (i == 1 && j == 1) || (i == 2 && j == 2))) {
        if (game_get_square(g, i, j) != GRASS) {
          game_delete(g);
          return false;
        }
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_game_won() {
  //  CAS 1 : Grille gagnante Avec BIGTREE Sans Wrapping

  game g1 = game_new_empty_ext(3, 3, false);
  if (g1 == NULL) return false;

  game_set_square(g1, 1, 1, BIGTREE);
  game_set_square(g1, 1, 0, TENT);
  game_set_square(g1, 1, 2, TENT);
  game_set_expected_nb_tents_row(g1, 0, 0);
  game_set_expected_nb_tents_row(g1, 1, 2);
  game_set_expected_nb_tents_row(g1, 2, 0);
  game_set_expected_nb_tents_col(g1, 0, 1);
  game_set_expected_nb_tents_col(g1, 1, 0);
  game_set_expected_nb_tents_col(g1, 2, 1);

  if (!game_won(g1)) {
    game_delete(g1);
    return false;
  }

  //  CAS 2 : GRILLE PERDANTE (BIGTREE avec 1 seule tente) Sans Wrapping

  game g2 = game_copy(g1);
  if (g2 == NULL) {
    game_delete(g1);
    return false;
  }
  game_set_square(g2, 1, 2, GRASS);  // enlève une tente

  if (game_won(g2)) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }

  //  CAS 3 : GRILLE GAGNANTE (TREE et TENT séparés mais voisins grâce au wrapping)

  game g3 = game_new_empty_ext(2, 3, true);
  if (g3 == NULL) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }

  game_set_square(g3, 0, 0, TREE);
  game_set_square(g3, 0, 2, TENT);
  game_set_expected_nb_tents_row(g3, 0, 1);
  game_set_expected_nb_tents_row(g3, 1, 0);
  game_set_expected_nb_tents_col(g3, 0, 0);
  game_set_expected_nb_tents_col(g3, 1, 0);
  game_set_expected_nb_tents_col(g3, 2, 1);

  game_print(g3);
  printf("is_neigh tent/tent diag = %d\n", game_is_neigh(g3, 0, 2, TENT, true));
  if (!game_won(g3)) {
    game_delete(g1);
    game_delete(g2);
    game_delete(g3);
    return false;
  }

  //  CAS 4 : GRILLE PERDANTE (meme grille que g3 mais sans wrapping)

  game g4 = game_new_empty_ext(2, 3, false);
  if (g4 == NULL) {
    game_delete(g1);
    game_delete(g2);
    game_delete(g3);
    return false;
  }

  game_set_square(g4, 0, 0, TREE);
  game_set_square(g4, 0, 2, TENT);
  game_set_expected_nb_tents_row(g4, 0, 1);
  game_set_expected_nb_tents_row(g4, 1, 0);
  game_set_expected_nb_tents_col(g4, 0, 0);
  game_set_expected_nb_tents_col(g4, 1, 0);
  game_set_expected_nb_tents_col(g4, 2, 1);

  if (game_won(g4)) {
    game_delete(g1);
    game_delete(g2);
    game_delete(g3);
    game_delete(g4);
    return false;
  }

  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  game_delete(g4);
  return true;
}

bool test_game_check_move() {
  game g = game_new_empty();
  game_set_square(g, 2, 2, TREE);

  if (!game_check_move(g, 0, 0, TENT)) {
    game_delete(g);
    return false;
  }
  if (game_check_move(g, 2, 2, TENT)) {
    game_delete(g);
    return false;
  }
  if (game_check_move(g, 1, 1, TREE)) {
    game_delete(g);
    return false;
  }
  if (game_check_move(g, 8, 8, TENT)) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_nb_rows() {
  game g = game_new_empty_ext(5, 7, false);
  if (game_nb_rows(g) != 5) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_nb_cols() {
  game g = game_new_empty_ext(5, 7, false);
  if (game_nb_cols(g) != 7) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_is_wrapping() {
  game g1 = game_new_empty_ext(5, 7, false);
  if (game_is_wrapping(g1)) {
    game_delete(g1);
    return false;
  }
  game_delete(g1);

  game g2 = game_new_empty_ext(5, 7, true);
  if (!game_is_wrapping(g2)) {
    game_delete(g2);
    return false;
  }
  game_delete(g2);
  return true;
}

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  if (argc == 1) usage(argc, argv);

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy", argv[1]) == 0)
    ok = test_dummy();
  else if (strcmp("game_is_neigh", argv[1]) == 0)
    ok = test_game_is_neigh();
  else if (strcmp("game_nb_neigh", argv[1]) == 0)
    ok = test_game_nb_neigh();
  else if (strcmp("game_get_adjacent_square", argv[1]) == 0)
    ok = test_game_get_adjacent_square();
  else if (strcmp("game_default_solution", argv[1]) == 0)
    ok = test_game_default_solution();
  else if (strcmp("game_default", argv[1]) == 0)
    ok = test_game_default();
  else if (strcmp("game_restart", argv[1]) == 0)
    ok = test_game_restart();
  else if (strcmp("game_fill_grass", argv[1]) == 0)
    ok = test_game_fill_grass();
  else if (strcmp("game_won", argv[1]) == 0)
    ok = test_game_won();
  else if (strcmp("game_check_move", argv[1]) == 0)
    ok = test_game_check_move();
  else if (strcmp("game_nb_rows", argv[1]) == 0)
    ok = test_game_nb_rows();
  else if (strcmp("game_nb_cols", argv[1]) == 0)
    ok = test_game_nb_cols();
  else if (strcmp("game_is_wrapping", argv[1]) == 0)
    ok = test_game_is_wrapping();
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