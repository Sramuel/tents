#ifndef __GAME_STRUCT_H__
#define __GAME_STRUCT_H__

#include "game.h"
#include "queue.h"

struct game_s {
  square **board;      // Grille principale: contient TREE,TENT,GRASS,EMPTY
  uint *nb_tents_row;  // Tableau du nombre attendu de tentes par ligne
  uint *nb_tents_col;  // Tableau du nombre attendu de tentes par colonne
  uint nb_rows;        // Nombre de lignes (par défaut DEFAULT_SIZE)
  uint nb_cols;        // Nombre de colonnes (par défaut DEFAULT_SIZE)
  bool wrapping;       // Est vrai si le jeu à une topologie torique (faux sinon)
  queue *undo;         // Pile des actions pour la fonctionnalité "undo"
  queue *redo;         // Pile des actions pour la fonctionnalité "redo"
};

typedef struct {  // structure pour stocker une action de jeu
  uint row;       // indice de la ligne
  uint col;       // indice de la colonne
  square value;   // valeur de la case avant/après l'action
} cell;

#endif