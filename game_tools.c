#include "game_tools.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "game_aux2.h"
#include "game_ext.h"

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
  if (game_get_square(g, pos % game_nb_cols(g), pos / game_nb_cols(g)) == TREE ||
      game_get_square(g, pos % game_nb_cols(g), pos / game_nb_cols(g)) == BIGTREE) {
    // On evite les arbres et gros arbres
    game_solve_aux(g, pos + 1, len, count, show);
  } else {
    if ((*count) == 1 && show) {
      return;
    } else {
      // On rempli la case avec de l'herbe
      game_play_move(g, pos % game_nb_cols(g), pos / game_nb_cols(g), GRASS);
    }

    if (game_winable(g)) game_solve_aux(g, pos + 1, len, count, show);

    if ((*count) == 1 && show) {
      return;
    } else {
      // On rempli la case avec une tente
      game_play_move(g, pos % game_nb_cols(g), pos / game_nb_cols(g), TENT);
    }

    if (game_winable(g)) game_solve_aux(g, pos + 1, len, count, show);

    if ((*count) == 1 && show) {
      return;
    }

    game_play_move(g, pos % game_nb_cols(g), pos / game_nb_cols(g), EMPTY);
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
