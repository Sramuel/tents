#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char* argv[]) {
  srand(time(NULL));
  if (argc < 7 || argc > 8) {
    fprintf(stderr, "Usage: %s <nb_rows> <nb_cols> <wrapping> <nb_trees> <nb_bigtrees> <solution> [<filename>]\n",
            argv[0]);
    return EXIT_FAILURE;
  }
  game g = game_random((uint)atoi(argv[1]), (uint)atoi(argv[2]), (bool)atoi(argv[3]), (uint)atoi(argv[4]),
                       (uint)atoi(argv[5]), (bool)atoi(argv[6]));  // Usage ./game_random <nb_rows> <nb_cols> <wrapping>
                                                                   // <nb_trees> <nb_bigtrees> <solution> [<filename>]
  if (argc == 8) {  // Sauvegarde si le nom de fichier est fourni (optionnel)
    game_save(g, argv[7]);
  }
  game_print(g);
  game_delete(g);
  return EXIT_SUCCESS;
}