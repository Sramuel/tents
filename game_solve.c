#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char* argv[]) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage: %s <option> <input> [<output>]\n", argv[0]);
    return EXIT_FAILURE;
  }
  game g = game_load(argv[2]);
  if (strcmp(argv[1], "-s") == 0) {
    if (!game_solve(g)){
      printf("No Solution\n");
      exit(EXIT_FAILURE); // Il n'y a pas de solution au jeu donc on quitte le programme
    } // Si on a une solution on l'affiche
    if (argc == 4) {  // Sauvegarde si le nom de fichier est fourni (optionnel)
      game_save(g, argv[3]);
    }
  } else if (strcmp(argv[1], "-c") == 0) {
    uint nb_sols = game_nb_solutions(g);
    if (argc == 4) {  // Sauvegarde si le nom de fichier est fourni (optionnel)
      FILE* f = fopen(argv[3], "w");
      if (f == NULL) {
        exit(EXIT_FAILURE);
      }
      fprintf(f, "%d\n", nb_sols);
      fclose(f);
    } else {
      printf("%d\n", nb_sols);
    }
  }
  game_delete(g);
  return EXIT_SUCCESS;
}