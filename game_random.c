#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char* argv[]) {
  srand(time(NULL));
  game g = game_random((uint)atoi(argv[1]), (uint)atoi(argv[2]), (bool)atoi(argv[3]), (uint)atoi(argv[4]),
                       (uint)atoi(argv[5]), (bool)atoi(argv[6]));
  game_save(g, argv[7]);
  game_print(g);
  game_delete(g);
  return EXIT_SUCCESS;
}