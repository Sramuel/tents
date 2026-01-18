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

bool test_game_is_neigh() {
  game g = game_new_empty();

  // On place des arbres autour de la position 3:3 
  game_set_square(g, 3, 3, TREE);
  game_set_square(g, 3, 2, TREE);
  game_set_square(g, 3, 4, TREE);
  game_set_square(g, 2, 3, TREE);
  game_set_square(g, 4, 3, TREE);
  game_set_square(g, 2, 2, TREE);
  game_set_square(g, 4, 4, TREE);
  
  // Puis on test a plusieurs emplacements si la fonction trouve bien des voisins ou non avec ou sans l'option diagonales
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
  // On initialise un nouveau jeu vide
  uint nb_rows = 8;
  uint nb_cols = 8;
  game g = game_new_empty_ext(nb_rows, nb_cols, true);

  // On y place trois TREE, deux TENT et une GRASS
  game_set_square(g, 0, 0, TREE);
  game_set_square(g, 1, 0, TREE);
  game_set_square(g, 2, 0, TREE);
  game_set_square(g, 0, 1, TENT);
  game_set_square(g, 2, 1, TENT);
  game_set_square(g, 2, 2, GRASS);

  // Puis on test si le nombre de voisin donnée a une position est le bon
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

  // On test pour chaque direction qu'on obtient bien la bonne case adjacente 
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

  for (uint i = 0; i < DEFAULT_SIZE; i++) { // On vérifie qu'il ne reste aucune case EMPTY
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(sol, i, j) == EMPTY) {
        game_delete(sol);
        game_delete(def);
        return false;
      }
    }
  }
  for (uint i = 0; i < DEFAULT_SIZE; i++) { // On vérifie qu'aucune TENT n'est adjacente avec une autre
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
      if (game_get_square(def, i, j) == TREE) { // On vérifie que chaque TREE est orthogonalement adjacent a au moins une TENT
        if (!game_is_neigh(sol, i, j, TENT, false)) {
          game_delete(sol);
          game_delete(def);
          return false;
        }
      }
    }
  }
  for (uint i = 0; i < DEFAULT_SIZE; i++) { // On compte le nombre de TENT sur une ligne
    uint nb_tent_row= 0;
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(sol, i, j) == TENT) {
        nb_tent_row++;
      }
    }
    if (nb_tent_row != game_get_expected_nb_tents_row(def, i)) {  // Et on compare ce nombre au nombre de TENT expected sur cette meme ligne
      game_delete(sol);
      game_delete(def);
      return false;
    }
  }
  for (uint j = 0; j < DEFAULT_SIZE; j++) { // On compte le nombre de TENT sur une colonne
    uint nb_tent_col = 0;
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
      if (game_get_square(sol, i, j) == TENT) {
        nb_tent_col++;
      }
    }
    if (nb_tent_col != game_get_expected_nb_tents_col(def, j)) {  // Et on compare ce nombre au nombre de TENT expected sur cette meme colonne
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

  for (uint i = 0; i < DEFAULT_SIZE; i++) { // game_default ne doit avoir que des cases TREE et EMPTY
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
  if (trees_def != trees_sol) { // Le nombre d'arbres entre game_default et sa solution doit être les mêmes
    game_delete(sol);
    game_delete(def);
    return false;
  }

  // Les expected_nb_tents doivent être les même pour chaque colonne et ligne
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
  // On place une TENT et une GRASS dans g_to_restart
  game_play_move(g_to_restart, 0, 1, TENT);
  game_play_move(g_to_restart, 1, 1, GRASS);
  if (game_equal(g_original, g_to_restart)) {
    game_delete(g_original);
    game_delete(g_to_restart);
    return false;
  }
  // Puis on s'assure que game_restart a réinitialisé la grille a son etat par default en la comparant a game_default
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

  // On test que game_fill_grass ne change que les cases EMPTY en GRASS
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
  for (uint i = 0; i < game_nb_rows(g); i++) {  // Puis on regarde si toutes les cases EMPTY ont bien été modifié en GRASS
    for (uint j = 0; j < game_nb_cols(g); j++) {
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
  // On créer 3 jeux dans les quels on brise plusieurs règles afin d'invalidé la solution 
  game g1 = game_default_solution(); // On s'assure d'abord que la solution par default est bien gagnante
  if (!game_won(g1)) {
    game_delete(g1);
    return false;
  }
  game g2 = game_default_solution();
  game g3 = game_default_solution();

  game_set_square(g1, 2, 2, TREE);
  game_set_square(g1, 3, 2, TENT);
  game_set_square(g1, 2, 6, TENT);
  game_set_square(g1, 3, 6, TREE);

  game_set_square(g2, 5, 2, GRASS);

  game_set_square(g3, 4, 4, GRASS);
  game_set_square(g3, 1, 3, TREE);
  game_set_square(g3, 0, 5, GRASS);
  game_set_square(g3, 5, 7, TREE);
  if (game_won(g1) || game_won(g2) || game_won(g3)) {
    game_delete(g1);
    game_delete(g2);
    game_delete(g3);
    return false;
  }
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  return true;

  // Nouveaux tests pour BIGTREE

  // new_g1 : une seule tente pour un BIGTREE (doit être PERDANT)
  game new_g1 = game_new_empty();
  if (!new_g1) return false;

  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    game_set_expected_nb_tents_row(new_g1, i, 0);
    game_set_expected_nb_tents_col(new_g1, i, 0);
  }
  game_set_square(new_g1, 3, 3, BIGTREE);
  game_set_square(new_g1, 3, 4, TENT);

  game_set_expected_nb_tents_row(new_g1, 3, 1);  // On attends une tente en 3:4
  game_set_expected_nb_tents_col(new_g1, 4, 1);

  game_fill_grass(new_g1);

  if (game_won(new_g1)) {  // Doit être faux : BIGTREE exige 2 tentes
    game_delete(new_g1);
    return false;
  }
  game_delete(new_g1);

  // new_g2 : BIGTREE avec 2 tentes (doit être GAGNANT)
  game new_g2 = game_new_empty();
  if (!new_g2) return false;

  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    game_set_expected_nb_tents_row(new_g2, i, 0);
    game_set_expected_nb_tents_col(new_g2, i, 0);
  }

  game_set_square(new_g2, 3, 3, BIGTREE);
  game_set_square(new_g2, 2, 4, TENT);
  game_set_square(new_g2, 4, 3, TENT);

  game_set_expected_nb_tents_row(new_g2, 2, 1);
  game_set_expected_nb_tents_row(new_g2, 4, 1);
  game_set_expected_nb_tents_col(new_g2, 3, 2);

  game_fill_grass(new_g2);

  if (!game_won(new_g2)) {  // Doit être vrai si toutes les règles sont OK
    game_delete(new_g2);
    return false;
  }
  game_delete(new_g2);

  return true;
}

bool test_game_check_move() {
  game g = game_new_empty();
  game_set_square(g, 2, 2, TREE);

  if (!game_check_move(g, 0, 0, TENT)) {  // Verifie qu'on peux bien placer une TENT sur une case EMPTY
    game_delete(g);
    return false;
  }
  if (game_check_move(g, 2, 2, TENT)) { // Verifie qu'on ne peux pas placer une TENT sur une case TREE
    game_delete(g);
    return false;
  }
  if (game_check_move(g, 1, 1, TREE)) { // Test de posé un TREE (ne devrait pas etre possible)
    game_delete(g);
    return false;
  }
  if (game_check_move(g, 8, 8, TENT)) { // Test de posé une TENT en dehors de la grille (pas possible non plus)
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_nb_rows() {
  // Créer un jeu avec un certain nombre de lignes ici 5 et test que game_nb_rows renvoie le même nombre
  game g = game_new_empty_ext(5, 7, false);
  if (game_nb_rows(g) != 5) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_nb_cols() {
  // Créer un jeu avec un certain nombre de colonnes ici 7 et test que game_nb_cols renvoie le même nombre
  game g = game_new_empty_ext(5, 7, false);
  if (game_nb_cols(g) != 7) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_game_is_wrapping() {
  // Créer une grille sans l'option wrapping et test que game_is_wrapping renvoie false
  game g1 = game_new_empty_ext(5, 7, false);
  if (game_is_wrapping(g1)) {
    game_delete(g1);
    return false;
  }
  game_delete(g1);
  // Créer une grille avec l'option wrapping et test que game_is_wrapping renvoie true
  game g2 = game_new_empty_ext(5, 7, true);
  if (!game_is_wrapping(g2)) {
    game_delete(g2);
    return false;
  }
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