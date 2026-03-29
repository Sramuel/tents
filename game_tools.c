#include "game_tools.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_aux2.h"
#include "game_ext.h"
#include "game_struct.h"

game game_load(char* filename) {
  if (filename == NULL) {
    return NULL;
  }
  FILE* f = fopen(filename, "r");
  if (f == NULL) {
    return NULL;
  }
  uint rows = 0;
  uint cols = 0;
  uint wrapped = 0;
  if (fscanf(f, "%d ", &rows) != 1) {  // lecture du nombre de lignes
    fclose(f);
    return NULL;
  }
  if (fscanf(f, "%d ", &cols) != 1) {  // lecture du nombre de colonnes
    fclose(f);
    return NULL;
  }
  if (fscanf(f, "%d ", &wrapped) != 1) {  // lecture du mode de jeu (wrapping ou non)
    fclose(f);
    return NULL;
  }
  game g = game_new_empty_ext(rows, cols, (bool)wrapped);
  if (g == NULL) {
    fclose(f);
    return NULL;
  }
  uint expected_tent = 0;
  for (uint i = 0; i < game_nb_rows(g); i++) {
    if (fscanf(f, "%d ", &expected_tent) != 1) {
      fclose(f);
      game_delete(g);
      return NULL;
    }
    game_set_expected_nb_tents_row(g, i, expected_tent);
  }
  for (uint j = 0; j < game_nb_cols(g); j++) {
    if (fscanf(f, "%d ", &expected_tent) != 1) {
      fclose(f);
      game_delete(g);
      return NULL;
    }
    game_set_expected_nb_tents_col(g, j, expected_tent);
  }
  for (uint i = 0; i < game_nb_rows(g); i++) {  // lecture de chaque case du jeu
    for (uint j = 0; j < game_nb_cols(g); j++) {
      char c;
      if (fscanf(f, "%c ", &c) != 1) {
        fclose(f);
        game_delete(g);
        return NULL;
      }
      if (c == 'X') {
        game_set_square(g, i, j, TREE);
      }
      if (c == 'E') {
        game_set_square(g, i, j, EMPTY);
      }
      if (c == 'G') {
        game_set_square(g, i, j, GRASS);
      }
      if (c == 'T') {
        game_set_square(g, i, j, TENT);
      }
      if (c == 'B') {
        game_set_square(g, i, j, BIGTREE);
      }
    }
  }
  fclose(f);
  return g;
}

void game_save(cgame g, char* filename) {
  if (g == NULL || filename == NULL) {
    return;
  }
  FILE* f = fopen(filename, "w");
  if (f == NULL) {
    return;
  }
  fprintf(f, "%d ", game_nb_rows(g));        //écriture du nombre de lignes
  fprintf(f, "%d ", game_nb_cols(g));        //écriture du nombre de colonnes
  fprintf(f, "%d \n", game_is_wrapping(g));  //écriture du mode de jeu (wrapping ou non)
  for (uint i = 0; i < game_nb_rows(g); i++) {
    fprintf(f, "%d ", game_get_expected_nb_tents_row(g, i));  //écriture du nombre de tentes attendu par ligne
  }
  fprintf(f, "\n");
  for (uint j = 0; j < game_nb_cols(g); j++) {
    fprintf(f, "%d ", game_get_expected_nb_tents_col(g, j));  //écriture du nombre de tentes attendu par colonne
  }
  fprintf(f, "\n");
  for (uint i = 0; i < game_nb_rows(g); i++) {  //écriture de chaque case du jeu
    for (uint j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == TREE) {
        fprintf(f, "X ");
      }
      if (game_get_square(g, i, j) == EMPTY) {
        fprintf(f, "E ");
      }
      if (game_get_square(g, i, j) == GRASS) {
        fprintf(f, "G ");
      }
      if (game_get_square(g, i, j) == TENT) {
        fprintf(f, "T ");
      }
      if (game_get_square(g, i, j) == BIGTREE) {
        fprintf(f, "B ");
      }
    }
    if (i + 1 != game_nb_rows(g)) fprintf(f, "\n");
  }
  fclose(f);
}

/* ************************************************************************** */
/*                                RANDOM                                      */
/* ************************************************************************** */

static int opposite_direction(direction d) {
  switch (d) {
    case UP:
      return DOWN;
    case DOWN:
      return UP;
    case LEFT:
      return RIGHT;
    case RIGHT:
      return LEFT;
    case UP_LEFT:
      return DOWN_RIGHT;
    case UP_RIGHT:
      return DOWN_LEFT;
    case DOWN_LEFT:
      return UP_RIGHT;
    case DOWN_RIGHT:
      return UP_LEFT;
    case HERE:
      return HERE;
  }
  return -1;
}

/* ************************************************************************** */

static bool try_to_place_tree(game g, bool big) {
  uint nb_rows = game_nb_rows(g);
  uint nb_cols = game_nb_cols(g);
  uint i_tree = rand() % nb_rows;
  uint j_tree = rand() % nb_cols;

  if (game_get_square(g, i_tree, j_tree) != EMPTY) return false;  // Case déjà occupée

  direction dir = (rand() % 4) + 1;  // UP, DOWN, LEFT, RIGHT
  uint i_tent, j_tent;
  bool ok = game_get_adjacent_square(g, i_tree, j_tree, dir, &i_tent, &j_tent);

  if (!ok) return false;                                           // Hors des limites
  if (game_get_square(g, i_tent, j_tent) != EMPTY) return false;   // Déjà occupée
  if (game_is_neigh(g, i_tent, j_tent, TENT, true)) return false;  // Adjacente à une autre tente

  // Pour un gros arbre (big tree), vérifier la case opposée pour la deuxième tente
  uint i_tent2, j_tent2;
  if (big) {
    direction opposite_dir = opposite_direction(dir);
    bool ok2 = game_get_adjacent_square(g, i_tree, j_tree, opposite_dir, &i_tent2, &j_tent2);
    if (!ok2) return false;
    if (game_get_square(g, i_tent2, j_tent2) != EMPTY) return false;   // Déjà occupée
    if (game_is_neigh(g, i_tent2, j_tent2, TENT, true)) return false;  // Adjacente à une autre tente
  }

  // Placer l'arbre et la première tente
  game_set_square(g, i_tree, j_tree, TREE);
  game_set_expected_nb_tents_row(g, i_tent, game_get_expected_nb_tents_row(g, i_tent) + 1);
  game_set_expected_nb_tents_col(g, j_tent, game_get_expected_nb_tents_col(g, j_tent) + 1);
  game_set_square(g, i_tent, j_tent, TENT);

  // Placer le gros arbre et la deuxième tente
  if (big) {
    game_set_square(g, i_tree, j_tree, BIGTREE);
    game_set_expected_nb_tents_row(g, i_tent2, game_get_expected_nb_tents_row(g, i_tent2) + 1);
    game_set_expected_nb_tents_col(g, j_tent2, game_get_expected_nb_tents_col(g, j_tent2) + 1);
    game_set_square(g, i_tent2, j_tent2, TENT);
  }

  return true;
}

/* ************************************************************************** */

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint nb_trees, uint nb_bigtrees, bool solution) {
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping);
  assert(g);

  // Essayer de placer les gros arbres en premier
  int num_bigtree = 0;
  int nb_bigtree_tries = 0;
  while (num_bigtree < nb_bigtrees) {
    if (nb_bigtree_tries >= 10 * nb_bigtrees) break;  // Éviter une boucle infinie
    nb_bigtree_tries++;
    if (try_to_place_tree(g, true)) num_bigtree++;
  }

  printf("Placé %d gros arbres (demandés %d) après %d tentatives\n", num_bigtree, nb_bigtrees, nb_bigtree_tries);

  // Essayer ensuite de placer les arbres normaux
  int num_tree = 0;
  int nb_tree_tries = 0;
  while (num_tree < nb_trees) {
    if (nb_tree_tries >= 10 * nb_trees) break;  // Éviter une boucle infinie
    nb_tree_tries++;
    if (try_to_place_tree(g, false)) num_tree++;
  }

  printf("Placé %d arbres (demandés %d) après %d tentatives\n", num_tree, nb_trees, nb_tree_tries);

  game_fill_grass(g);
  assert(game_won(g));
  if (!solution) game_restart(g);
  return g;
}
/* ************************************************************************** */

bool game_winable(cgame g) {
  assert(g);

  // Règles 1 : Pas de tentes adjacentes même diagonalement
  for (uint i = 0; i < game_nb_rows(g); i++)
    for (uint j = 0; j < game_nb_cols(g); j++)
      if ((game_get_square(g, i, j) == TENT) && game_is_neigh(g, i, j, TENT, true)) return false;

  // verification par ligne : pas trop de tente et tent_expected pas "impossible a compléter"
  for (uint i = 0; i < game_nb_rows(g); i++) {
    int nb_expected_tents_row = game_get_expected_nb_tents_row(g, i);
    int nb_current_tents_row = game_get_current_nb_tents_row(g, i);
    if (nb_current_tents_row > nb_expected_tents_row) return false;

    // on compte le nombre de case vide restantes pour savoir si le expected est possible
    int empty = 0;
    for (uint j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == EMPTY) empty++;
    }
    if (nb_current_tents_row + empty < nb_expected_tents_row) return false;
  }

  // pareil pour les colonnes
  for (uint j = 0; j < game_nb_cols(g); j++) {
    int nb_expected_tents_col = game_get_expected_nb_tents_col(g, j);
    int nb_current_tents_col = game_get_current_nb_tents_col(g, j);
    if (nb_current_tents_col > nb_expected_tents_col) return false;

    int empty = 0;
    for (uint i = 0; i < game_nb_rows(g); i++) {
      if (game_get_square(g, i, j) == EMPTY) empty++;
    }
    if (nb_current_tents_col + empty < nb_expected_tents_col) return false;
  }

  // Régle 4 : Chaque tente doit toucher un arbre (ou bigtree) orthogonalement.
  for (uint i = 0; i < game_nb_rows(g); i++)
    for (uint j = 0; j < game_nb_cols(g); j++) {
      // chaque tente devrait deja avoir au moins un voisin TREE ou BIGTREE orthogonalement.
      if (game_get_square(g, i, j) == TENT) {
        if (!game_is_neigh(g, i, j, TREE, false) && !game_is_neigh(g, i, j, BIGTREE, false)) return false;
      }

      // On verifie qu'au moins une tente potentielle peut encore être placé pres d'un arbre.
      if (game_get_square(g, i, j) == TREE) {
        uint nb_tents = game_nb_neigh(g, i, j, TENT, false);
        if (nb_tents == 0) {
          uint nb_empty = game_nb_neigh(g, i, j, EMPTY, false);
          if (nb_empty == 0) return false;  // Pas de tente deja placé et pas de place pour une nouvelle.
        }
      }

      // On verifie la meme chose pour BIGTREE avec exactement 2 tentes potentielles
      if (game_get_square(g, i, j) == BIGTREE) {
        uint nb_tents = game_nb_neigh(g, i, j, TENT, false);
        if (nb_tents > 2) return false;  // Faux si plus de 2 tentes

        uint nb_empty = game_nb_neigh(g, i, j, EMPTY, false);
        if (nb_tents + nb_empty < 2)
          return false;  // Manque de place pour avoir exactement 2 tentes orthogonalement adjacentes
      }
    }
  return true;
}

void game_solve_aux(game g, uint pos, uint len, uint* count, bool show) {
  if ((*count) == 1 && show) {
    return;
  }
  // Arret des appels recursifs
  if (pos == len) {
    if (game_won(g)) {
      (*count)++;
      if (show) {
        // Affichage de la game gagnée si c'est pour la fonction game_solve
        game_print(g);
      }
    }
    return;
  }
  // A chaque appel recursif, la pos est incrementee, on calcule ensuite le i et le j en fonction de la pos et de la
  // largeur de la game soit le nombre de colonnes
  if (game_get_square(g, pos / game_nb_cols(g), pos % game_nb_cols(g)) == TREE ||
      game_get_square(g, pos / game_nb_cols(g), pos % game_nb_cols(g)) == BIGTREE) {
    // On evite les arbres et gros arbres
    game_solve_aux(g, pos + 1, len, count, show);
  } else {
    if ((*count) == 1 && show) {
      return;
    } else {
      // On rempli la case avec de l'herbe
      game_play_move(g, pos / game_nb_cols(g), pos % game_nb_cols(g), GRASS);
    }

    if (game_winable(g)) game_solve_aux(g, pos + 1, len, count, show);

    if ((*count) == 1 && show) {
      return;
    } else {
      // On rempli la case avec une tente
      game_play_move(g, pos / game_nb_cols(g), pos % game_nb_cols(g), TENT);
    }

    if (game_winable(g)) game_solve_aux(g, pos + 1, len, count, show);

    if ((*count) == 1 && show) {
      return;
    }

    game_play_move(g, pos / game_nb_cols(g), pos % game_nb_cols(g), EMPTY);
  }
}

bool game_solve(game g) {
  uint len = game_nb_rows(g) * game_nb_cols(g);
  uint pos = 0;
  uint count = 0;
  bool show = true;
  game_solve_aux(g, pos, len, &count, show);
  return count == 1;
}

uint game_nb_solutions(cgame g) {
  uint len = game_nb_rows(g) * game_nb_cols(g);
  uint pos = 0;
  uint count = 0;
  bool show = false;
  game_solve_aux((game)g, pos, len, &count, show);
  return count;
}
