#define __USE_GNU
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"

/* ********** TEST ********** */

bool test_dummy(){
    return true;
}

bool test_new(){
    square* squares = malloc((DEFAULT_SIZE*DEFAULT_SIZE)*sizeof(square));
    if (squares==NULL){
        fprintf(stderr,"Erreur d'Allocation\n");
        exit(EXIT_FAILURE);
    }
    uint nb_tents_row[DEFAULT_SIZE];
    uint nb_tents_col[DEFAULT_SIZE];
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        nb_tents_row[i] = 0;
        nb_tents_col[i] = 0;
    }
    for (uint i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
        squares[i] = EMPTY;
    }
    game g = game_new(squares, nb_tents_row, nb_tents_col);
    if (g == NULL){
        fprintf(stderr, "La fonction renvoie un pointeur Null\n");
        return false;
    }
    game_print(g);
    game_delete(g);
    free(squares);
    return true;
}

/* ********** USAGE ********** */

void usage(int argc, char *argv[])
{
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[])
{
  if (argc == 1)
    usage(argc, argv);

  // start test
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy", argv[1]) == 0)
    ok = test_dummy();
  else if (strcmp("new", argv[1]) == 0)
    ok = test_new();


  else
  {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // print test result
  if (ok)
  {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  }
  else
  {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
