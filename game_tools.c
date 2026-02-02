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
  // 1. CRÉATION DU JEU VIDE
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping);
  if (g == NULL) {
    exit(EXIT_FAILURE);
  }

  // Vérification basique : assez de place pour les arbres et tentes
  // Un BIGTREE a besoin de 3 cases (lui + 2 tentes)
  // Un TREE a besoin de 2 cases (lui + 1 tente)
  if ((nb_bigtrees * 3 + nb_trees * 2) > nb_cols * nb_rows) {
    exit(EXIT_FAILURE);
  }

  uint trees = nb_trees;
  uint big_trees = nb_bigtrees;

  // Compteurs de tentatives pour éviter boucle infinie (si le placement est impossible au bout de 100 essais, on
  // abandonne mais le jeu est quand même retourné avec ce qui a pu être placé)
  uint try_tree = 0;
  uint try_tree_max = 100;

  // 2. BOUCLE PRINCIPALE DE PLACEMENT DES ARBRES ET TENTES
  // On continue tant qu'il reste des arbres à placer (le nombre voulu par l'utilisateur) et qu'on n'a pas dépassé les
  // essais max
  while ((trees > 0 || big_trees > 0) && try_tree < try_tree_max) {
    try_tree++;
    square choix = tree_random();  // On choisit aléatoirement si on pose un TREE ou un BIGTREE
    if (trees == 0) {
      choix = BIGTREE;  // Si plus de TREE à placer, on choisit un BIGTREE
    } else if (big_trees == 0) {
      choix = TREE;  // Si plus de BIGTREE à placer, on choisit un TREE
    }

    // On choisit une case au hasard sur la grille
    uint row_rand = nb_random(nb_rows);
    uint col_rand = nb_random(nb_cols);

    if (game_get_square(g, row_rand, col_rand) !=
        EMPTY) {  // Sécurité pour éviter de mettre un arbre au même endroit qu'un autre arbre ou une tente
      continue;   // On passe à l'essai suivant
    }

    // 1er cas : on place un TREE et sa tente associée
    if (choix == TREE) {
      uint try_dir = 0;
      while (try_dir < 50) {  // Nombre de tentatives pour trouver une bonne direction totalement aléatoire (pour éviter
                              // un pattern fixe)
        direction d = rand() % 4 + 1;  // Choix aléatoire d'une direction parmi UP, DOWN, LEFT, RIGHT
        uint i;
        uint j;

        // LE WRAPPING EST GÉRÉ PAR LA FONCTION game_get_adjacent_square
        // success sera false si on sort de la grille (sans wrapping), voilà pourquoi on stocke son résultat
        bool success = game_get_adjacent_square(g, row_rand, col_rand, d, &i, &j);

        // VÉRIFICATIONS STRICTES :
        // 1. success : On est bien dans la grille
        // 2. EMPTY : La place pour la tente est libre
        // 3. game_check_move : Le placement de la tente est légal (pas adjacent à une autre tente, etc...)
        // 4. !game_is_neigh : Aucune autre tente autour (même en diagonale)
        if (success && game_get_square(g, i, j) == EMPTY && game_check_move(g, i, j, TENT) &&
            !game_is_neigh(g, i, j, TENT, true)) {
          // Tout est bon, on valide et on pose la tente et l'arbre
          game_set_square(g, i, j, TENT);                 // pose de la tente
          game_set_square(g, row_rand, col_rand, choix);  // pose de l'arbre
          // Mise à jour des compteurs de tentes attendues pour la ligne et la colonne concernées
          uint nb_tents_row =
              game_get_expected_nb_tents_row(g, i);  // Récupération du nombre actuel de tentes sur la ligne
          game_set_expected_nb_tents_row(g, i, nb_tents_row + 1);  // Incrémentation de ce nombre
          uint nb_tents_col =
              game_get_expected_nb_tents_col(g, j);  // Récupération du nombre actuel de tentes sur la colonne
          game_set_expected_nb_tents_col(g, j, nb_tents_col + 1);  // Incrémentation de ce nombre

          trees--;  // Décrémentation du nombre de TREE restants à placer
          break;    // Sortie de la boucle de tentative de direction
        }
        try_dir++;  // Incrémentation du compteur de tentatives si la direction choisie n'a pas fonctionné
      }
    }

    // 2e cas : on place un BIGTREE et ses 2 tentes associées
    else {
      uint try_dir = 0;
      while (try_dir < 50) {  // nombre de tentatives pour trouver une bonne direction
        direction d = rand() % 4 + 1;
        direction d_opp = opp_simple(d);  // direction opposée à d donc si d est UP, d_opp est DOWN, etc.
        uint i, i_opp;
        uint j, j_opp;

        // LE WRAPPING EST GÉRÉ PAR LA FONCTION game_get_adjacent_square
        // success sera false si on sort de la grille (sans wrapping), voilà pourquoi on stocke son résultat
        // On en prend deux, un dans la direction d et un dans la direction opposée d_opp pour faire ce schéma : *#* ou
        // l'inverse de haut en bas (les seuls endroits possibles pour placer les tentes autour d'un BIGTREE)
        bool success = game_get_adjacent_square(g, row_rand, col_rand, d, &i, &j);
        bool success_opp = game_get_adjacent_square(g, row_rand, col_rand, d_opp, &i_opp, &j_opp);

        // VÉRIFICATION DOUBLE :
        // Il faut que la tente 1 et la tente 2 soient valides simultanément
        // On vérifie EMPTY, check_move et is_neigh pour les deux cotés
        if (success && game_get_square(g, i, j) == EMPTY && game_check_move(g, i, j, TENT) && success_opp &&
            game_get_square(g, i_opp, j_opp) == EMPTY && game_check_move(g, i_opp, j_opp, TENT) &&
            !game_is_neigh(g, i, j, TENT, true) && !game_is_neigh(g, i_opp, j_opp, TENT, true)) {
          // Tout est bon, on valide et on pose les tentes et le BIGTREE
          game_set_square(g, i, j, TENT);                 // pose de la tente 1
          game_set_square(g, i_opp, j_opp, TENT);         // pose de la tente 2
          game_set_square(g, row_rand, col_rand, choix);  // pose du BIGTREE

          // Mise à jour des compteurs de tentes attendues pour les lignes et colonnes concernées
          uint nb_tents_row = game_get_expected_nb_tents_row(
              g, i);  // Récupération du nombre actuel de tentes sur la ligne de la tente 1
          game_set_expected_nb_tents_row(g, i, nb_tents_row + 1);  // Incrémentation de ce nombre pour la tente 1
          uint nb_tents_col = game_get_expected_nb_tents_col(
              g, j);  // Récupération du nombre actuel de tentes sur la colonne de la tente 1
          game_set_expected_nb_tents_col(g, j, nb_tents_col + 1);  // Incrémentation de ce nombre pour la tente 1
          nb_tents_row = game_get_expected_nb_tents_row(
              g, i_opp);  // Récupération du nombre actuel de tentes sur la ligne de la tente 2
          game_set_expected_nb_tents_row(g, i_opp, nb_tents_row + 1);  // Incrémentation de ce nombre pour la tente 2
          nb_tents_col = game_get_expected_nb_tents_col(
              g, j_opp);  // Récupération du nombre actuel de tentes sur la colonne de la tente 2
          game_set_expected_nb_tents_col(g, j_opp, nb_tents_col + 1);  // Incrémentation de ce nombre pour la tente 2
          big_trees--;  // Décrémentation du nombre de BIGTREE restants à placer
          break;        // Sortie de la boucle de tentative de direction
        }
        try_dir++;  // Incrémentation du compteur de tentatives si la direction choisie n'a pas fonctionné
      }
    }
  }

  // 3. FINALISATION DU JEU
  if (solution) {
    // Si on veut la solution : on remplit les vides avec de l'herbe
    game_fill_grass(g);
  } else {
    // Sinon on enlève toutes les tentes placées pour ne garder que les arbres ce qui constitue le jeu à résoudre
    for (uint i = 0; i < nb_rows; i++) {
      for (uint j = 0; j < nb_cols; j++) {
        if (game_get_square(g, i, j) == TENT) {
          game_set_square(g, i, j, EMPTY);  // On enlève la tente pour ne garder que les arbres
        }
      }
    }
  }
  return g;
}