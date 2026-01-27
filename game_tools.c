#include "game_tools.h"

#include <stdlib.h>
#include <time.h>

#include "game_aux2.h"
#include "game_ext.h"

game game_load(char* filename) {
  if (filename == NULL) {
    exit(EXIT_FAILURE);
  }
  FILE* f = fopen(filename, "r");
  if (f == NULL) {
    exit(EXIT_FAILURE);
  }
  uint rows = 0;
  uint cols = 0;
  uint wrapped = 0;
  fscanf(f, "%d ", &rows);     // lecture du nombre de lignes
  fscanf(f, "%d ", &cols);     // lecture du nombre de colonnes
  fscanf(f, "%d ", &wrapped);  // lecture du mode de jeu (wrapping ou non)
  game g = game_new_empty_ext(rows, cols, (bool)wrapped);
  if (g == NULL) {
    fclose(f);
    exit(EXIT_FAILURE);
  }
  uint expected_tent = 0;
  for (uint i = 0; i < game_nb_rows(g); i++) {
    fscanf(f, "%d ", &expected_tent);  // lecture du nombre de tentes attendu par ligne
    game_set_expected_nb_tents_row(g, i, expected_tent);
  }
  for (uint j = 0; j < game_nb_cols(g); j++) {
    fscanf(f, "%d ", &expected_tent);  // lecture du nombre de tentes attendu par ligne
    game_set_expected_nb_tents_col(g, j, expected_tent);
  }
  for (uint i = 0; i < game_nb_rows(g); i++) {  // lecture de chaque case du jeu
    for (uint j = 0; j < game_nb_cols(g); j++) {
      char c;
      fscanf(f, "%c ", &c);
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
    exit(EXIT_FAILURE);
  }
  FILE* f = fopen(filename, "w");
  if (f == NULL) {
    exit(EXIT_FAILURE);
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

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint nb_trees, uint nb_bigtrees, bool solution) {
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping);
  if (g==NULL){
    exit(EXIT_FAILURE);
  }
  srand(time(NULL));
  uint trees = nb_trees;
  uint big_trees = nb_bigtrees;

  while (trees != 0 || big_trees !=0) {
    square choix;
    if (trees == 0){
      square choix = BIGTREE;
    } else if (big_trees == 0) {
      square choix = TREE;
    } else {
      square choix = tree_random();
    }
    uint row_rand=nb_random(nb_rows);
    uint col_rand=nb_random(nb_cols);

    if (game_get_square(g,row_rand,col_rand)==TREE || game_get_square(g,row_rand,col_rand)==BIGTREE ){ //Sécurité pour éviter de mettre le nouvel abre au même endroit
      continue;
    }
    game_set_square(g, row_rand, col_rand, choix);
    if (choix == TREE){
      trees--;
    } else {
      big_trees--;
    }
  }
  return g;
}