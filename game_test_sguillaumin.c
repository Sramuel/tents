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

bool test_get_expected_nb_tents_all(){
  game g=game_default();
  if (g ==NULL)
    return false;
  uint val=0;
  for (uint i=0;i<DEFAULT_SIZE;i++){
    val+=game_get_expected_nb_tents_row(g,i);
  }
  uint total=game_get_expected_nb_tents_all(g);
  game_delete(g);
  if (val==total){
    return true;
  }
  return false;
}

bool test_get_current_nb_tents_col(){
  game g=game_new_empty();
  if (g ==NULL) 
    return false;
  game_set_square(g,0,0,TREE);
  game_set_square(g,2,6,TREE);
  game_set_square(g,6,7,TREE);
  for (uint i=0;i<DEFAULT_SIZE;i++){
    if (game_get_current_nb_tents_col(g,i)!=0){
      game_delete(g);
      return false;
    }
  }
  game_play_move(g,0,1,TENT);
  game_play_move(g,2,5,TENT);
  game_play_move(g,6,6,TENT);
  if (game_get_current_nb_tents_col(g,1) != 1) { game_delete(g); return false; }
  if (game_get_current_nb_tents_col(g,5) != 1) { game_delete(g); return false; }
  if (game_get_current_nb_tents_col(g,6) != 1) { game_delete(g); return false; }
  for(uint i=0;i<DEFAULT_SIZE;i++){ //je teste une nouvelle fois pour etre sur que les autres colonnes n'ont pas bougé
    if (i!=1 && i!=5 && i!=6){
      if (game_get_current_nb_tents_col(g,i)!=0){
        game_delete(g);
        return false;
      }
    }
  }
  game_set_square(g,4,1,TREE);
  game_play_move(g,5,1,TENT);
  if (game_get_current_nb_tents_col(g,1) != 2) { game_delete(g); return false; }
  game_delete(g);
  return true;
}

bool test_get_current_nb_tents_row(){
  game g=game_new_empty();
  if (g ==NULL) 
    return false;
  game_set_square(g,0,0,TREE);
  game_set_square(g,2,6,TREE);
  game_set_square(g,6,7,TREE);
  for (uint i=0;i<DEFAULT_SIZE;i++){
    if (game_get_current_nb_tents_row(g,i)!=0){
      game_delete(g);
      return false;
    }
  }
  game_play_move(g,0,1,TENT);
  game_play_move(g,2,5,TENT);
  game_play_move(g,6,6,TENT);
  if (game_get_current_nb_tents_row(g,0) != 1) { game_delete(g); return false; }
  if (game_get_current_nb_tents_row(g,2) != 1) { game_delete(g); return false; }
  if (game_get_current_nb_tents_row(g,6) != 1) { game_delete(g); return false; }
  for(uint i=0;i<DEFAULT_SIZE;i++){ //je teste une nouvelle fois pour etre sur que les autres lignes n'ont pas bougé
    if (i!=0 && i!=2 && i!=6){
      if (game_get_current_nb_tents_row(g,i)!=0){
        game_delete(g);
        return false;
      }
    }
  }
  game_set_square(g,0,4,TREE);
  game_play_move(g,0,5,TENT);
  if (game_get_current_nb_tents_row(g,0) != 2) { game_delete(g); return false; }
  game_delete(g);
  return true;
}

bool test_get_current_nb_tents_all(){
  game g=game_default();
  if (g ==NULL) 
    return false;
  if (game_get_current_nb_tents_all(g)!=0){
    game_delete(g);
    return false;
  }
  game_play_move(g,4,3,TENT);
  if (game_get_current_nb_tents_all(g)!=1){
    game_delete(g);
    return false;
  }
  game_play_move(g,0,1,TENT);
  if (game_get_current_nb_tents_all(g)!=2){
    game_delete(g);
    return false;
  }
  game_play_move(g,0,1,EMPTY);
  if (game_get_current_nb_tents_all(g)!=1){
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_play_move(){
  game g=game_default();
  if (g ==NULL) 
    return false;
  game_play_move(g,4,3,TENT);
  if (game_get_square(g,4,3)!=TENT){
    game_delete(g);
    return false;
  }
  game_play_move(g,4,3,EMPTY);
  if (game_get_square(g,4,3)!=EMPTY){
    game_delete(g);
    return false;
  }
  game_play_move(g,0,1,GRASS);
  if (game_get_square(g,0,1)!=GRASS){
    game_delete(g);
    return false;
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
  else if (strcmp("get_expected_nb_tents_all", argv[1]) == 0)
    ok = test_get_expected_nb_tents_all();
  else if (strcmp("get_current_nb_tents_col", argv[1]) == 0)
    ok = test_get_current_nb_tents_col();
  else if (strcmp("get_current_nb_tents_row", argv[1]) == 0)
    ok = test_get_current_nb_tents_row();
  else if (strcmp("get_current_nb_tents_all", argv[1]) == 0)
    ok = test_get_current_nb_tents_all();
  else if (strcmp("play_move", argv[1]) == 0)
    ok = test_play_move();
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
