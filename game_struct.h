#ifndef __GAME_STRUCT_H__
#define __GAME_STRUCT_H__

#include "game.h"

struct game_s {
  square **board;      // Grille principale: contient TREE,TENT,GRASS,EMPTY
  uint *nb_tents_row;  // Tableau du nombre attendu de tentes par ligne
  uint *nb_tents_col;  // Tableau du nombre attendu de tentes par colonne
  uint nb_rows;        // Nombre de lignes (par défaut DEFAULT_SIZE)
  uint nb_cols;        // Nombre de colonnes (par défaut DEFAULT_SIZE)
};

#endif