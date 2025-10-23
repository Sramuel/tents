#define __USE_GNU
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"

/* ********** TEST ********** */

bool test_dummy()
{
  return true;
}

bool test_print(){
  game g = game_default();
  game_print(g);
  game_delete(g);
  return true;
}

bool test_delete(){
  game g = game_default();
  game_delete(g);
  return true;
}

bool test_get_expected_nb_tents_row(){
  game g=game_default();
  if (g ==NULL) 
    return false;
  uint val[DEFAULT_SIZE]={1,2,1,2,2,0,1,3};
  for (uint i=0;i<DEFAULT_SIZE;i++){
    if (game_get_expected_nb_tents_row(g,i)!=val[i]){
      game_delete(g);
      return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_get_expected_nb_tents_col(){
  game g=game_default();
  if (g ==NULL) 
    return false;
  uint val[DEFAULT_SIZE]={3,0,2,1,2,1,2,1};
  for (uint j=0;j<DEFAULT_SIZE;j++){
    if (game_get_expected_nb_tents_col(g,j)!=val[j]){
      game_delete(g);
      return false;
    }
  }
  game_delete(g);
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
  else if (strcmp("print", argv[1]) == 0)
    ok = test_print();
  else if (strcmp("delete", argv[1]) == 0)
    ok = test_delete();
  else if (strcmp("get_expected_nb_tents_row", argv[1]) == 0)
    ok = test_get_expected_nb_tents_row();
  else if (strcmp("get_expected_nb_tents_col", argv[1]) == 0)
    ok = test_get_expected_nb_tents_col();
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
