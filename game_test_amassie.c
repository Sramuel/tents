#define __USE_GNU
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"


bool test_dummy(){
    return true;
}

bool test_game_is_neigh(){
  game g= game_new_empty();

  game_set_square(g, 3, 3, TREE);
  game_set_square(g, 3, 2, TREE);
  game_set_square(g, 3, 4, TREE);
  game_set_square(g, 2, 3, TREE);
  game_set_square(g, 4, 3, TREE);
  game_set_square(g, 2, 2, TREE);
  game_set_square(g, 4, 4, TREE);

  bool test1 = game_is_neigh(g, 3,3, TREE, false);

  bool test2 = !game_is_neigh(g, 0,0, TREE, false);

  bool test3 = !game_is_neigh(g, 1,1, TREE, false);
  bool test4 = game_is_neigh(g, 1,1, TREE, true);

  game_set_square(g, 7, 7, GRASS);
  bool test5 = !game_is_neigh(g, 7,6, TREE, false);
  game_delete(g);
  return test1 && test2 && test3 && test4 && test5;
}

bool test_game_get_ajacent_square(){
  game g= game_new_empty();
  uint i_next;
  uint j_next;

  if (!game_get_ajacent_square(g, 3 , 3, UP, &i_next, &j_next))return false;
  if (i_next !=2 || j_next !=3) return false;

  if (!game_get_ajacent_square(g, 3 , 3, LEFT, &i_next, &j_next))return false;
  if (i_next !=3 || j_next !=2) return false;

  if (!game_get_ajacent_square(g, 3 , 3, DOWN_RIGHT, &i_next, &j_next))return false;
  if (i_next !=4|| j_next !=4) return false;

  if (!game_get_ajacent_square(g, 3 , 3, DOWN, &i_next, &j_next))return false;
  if (i_next !=4 || j_next !=3) return false;

  if (game_get_ajacent_square(g, 0, 0, UP, &i_next, &j_next))return false;
  if (game_get_ajacent_square(g, 0, 0, LEFT, &i_next, &j_next))return false;
  if (game_get_ajacent_square(g, 0, 0, UP_LEFT, &i_next, &j_next))return false;
  if (game_get_ajacent_square(g, 0, 0, UP_RIGHT, &i_next, &j_next))return false;
  if (game_get_ajacent_square(g, 7, 7, RIGHT, &i_next, &j_next))return false;
  if (game_get_ajacent_square(g, 7, 7, DOWN_LEFT, &i_next, &j_next))return false;
  if (game_get_ajacent_square(g, 7, 7, DOWN_RIGHT, &i_next, &j_next))return false;
  if (game_get_ajacent_square(g, 7, 7, DOWN, &i_next, &j_next))return false;
  game_delete(g);
  return true;
}

bool test_game_default_solution(){
  game sol=game_default_solution();
  game def = game_default();

  for (uint i=0; i<DEFAULT_SIZE; i++){
    for (uint j=0; j<DEFAULT_SIZE; j++){
      if (game_get_square(sol,i,j)==EMPTY){
        fprintf(stderr, "Toute les cases ne sont pas remplies\n");
        game_delete(sol);
        game_delete(def);
        return false;
      }
    }
  }for (uint i=0; i<DEFAULT_SIZE; i++){
    for (uint j=0; j<DEFAULT_SIZE; j++){
      if (game_get_square(sol,i,j)==TENT){
        if(game_is_neigh(sol,i,j,TENT,true)){
          fprintf(stderr, "deux tentes sont adjacentes(attention diagonalement compte\n");
          game_delete(sol);
          game_delete(def);
          return false;
        }
      }
    }
  }for (uint i=0; i<DEFAULT_SIZE; i++){
    for (uint j=0; j<DEFAULT_SIZE; j++){
      if (game_get_square(def,i,j)==TREE){
        if(!game_is_neigh(sol,i,j,TENT,false)){
          fprintf(stderr, "un arbre ou plus n'as pas de tente a ses cotés\n");
          game_delete(sol);
          game_delete(def);
          return false;
        }
      }
    }
  }
  for (uint i=0; i<DEFAULT_SIZE; i++){
    uint nb=0;
    for (uint j=0; j<DEFAULT_SIZE; j++){
      if (game_get_square(sol,i,j)==TENT){
        nb++;
      }
    }if (nb!=game_get_expected_nb_tents_row(def, i)){
      fprintf(stderr, "le nombre de tentes dans chaque ligne est differentes de celui attendu\n");
      game_delete(sol);
      game_delete(def);
      return false;
    }
  }
  for (uint j=0; j<DEFAULT_SIZE; j++){
    uint nb=0;
    for (uint i=0; i<DEFAULT_SIZE; i++){
      if (game_get_square(sol,i,j)==TENT){
        nb++;
      }
    }if (nb!=game_get_expected_nb_tents_col(def, j)){
      fprintf(stderr, "le nombre de tentes dans chaque colonne est differentes de celui attendu\n");
      game_delete(sol);
      game_delete(def);
      return false;
    }
  }
  game_delete(sol);
  game_delete(def);
  return true;
}

bool test_game_default(){
  game def=game_default();
  game sol=game_default_solution();

  for (uint i=0; i<DEFAULT_SIZE; i++){
    for (uint j=0; j<DEFAULT_SIZE; j++){
      if (game_get_square(def,i,j)!=TREE && game_get_square(def,i,j)!=EMPTY){
        fprintf(stderr, "certaines case sont TENTS OU GRASS\n");
        game_delete(sol);
        game_delete(def);
        return false;
      }
    }
  }uint trees_def=0;
  uint trees_sol=0;

  for (uint i=0; i<DEFAULT_SIZE; i++){
    for (uint j=0; j<DEFAULT_SIZE; j++){
      if (game_get_square(def,i,j)==TREE)
        trees_def++;
      if (game_get_square(sol,i,j)==TREE)
        trees_sol++;
    }
  }if(trees_def!=trees_sol){
        fprintf(stderr, "mauvais nombre d'arbre sur le tableau de jeu\n");
        game_delete(sol);
        game_delete(def);
        return false;
  }
  for (uint i=0;i<DEFAULT_SIZE;i++){
    if(game_get_expected_nb_tents_row(def,i)!=game_get_expected_nb_tents_row(sol, i)){
      game_delete(def);
      game_delete(sol);
      return false;
    }
  }for (uint j=0;j<DEFAULT_SIZE;j++){
    if(game_get_expected_nb_tents_col(def,j)!=game_get_expected_nb_tents_col(sol, j)){
      game_delete(def);
      game_delete(sol);
      return false;
    }
  }
  game_delete(def);
  game_delete(sol);
  return true;
}

bool test_game_restart() {
  game g_original = game_default();
  if (g_original ==NULL){
    return false;
  }
  game g_to_restart =game_copy(g_original);
  if (g_to_restart ==NULL){
    game_delete(g_original);
    return false;
  }
  game_play_move(g_to_restart,0,1, TENT);
  game_play_move(g_to_restart,1,1, GRASS);
  if (game_equal(g_original,g_to_restart)) {
    game_delete(g_original);
    game_delete(g_to_restart);
    return false;
  }
  game_restart(g_to_restart);
  bool test_passed =game_equal(g_original,g_to_restart);
  if (!test_passed){
    game_delete(g_original);
    game_delete(g_to_restart);
    return test_passed;
  }
  game_delete(g_original);
  game_delete(g_to_restart);
  return test_passed;
}

bool test_game_fill_grass(){
  game g= game_new_empty();
  game_set_square(g,0,0,TREE);
  game_set_square(g,1,1,TENT);
  game_set_square(g,2,2,GRASS);
  game_fill_grass(g);

  if (game_get_square(g,0,0)!=TREE){
    fprintf(stderr, "un TREE a été changé en GRASS\n");
    game_delete(g);
    return false;
  }if (game_get_square(g,1,1)!=TENT){
    fprintf(stderr, "une TENT a été changé en GRASS\n");
    game_delete(g);
    return false;
  }if (game_get_square(g,2,2)!=GRASS){
    fprintf(stderr, "une GRASS a été changé en en autre chose\n");
    game_delete(g);
    return false;
  }
  for (uint i=0; i<DEFAULT_SIZE; i++){
    for (uint j=0; j<DEFAULT_SIZE; j++){
      if (!((i==0 &&j==0) || (i==1 &&j==1)|| (i==2 && j==2))){
        if(game_get_square(g,i,j)!=GRASS){
          game_delete(g);
          return false;
        }
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_game_won(){
  game sol=game_default_solution();
  game def=game_default();
  if (!game_won(sol)){
    fprintf(stderr, "solution devrait etre gagnante\n");
    game_delete(sol);
    game_delete(def);
    return false;
  }
  if (game_won(def)){
    fprintf(stderr, "jeu par default ne devrait pas etre gagnant\n");
    game_delete(sol);
    game_delete(def);
    return false;
  }
  game_set_square(sol,0,0,TENT);
  if(game_won(sol)){
    fprintf(stderr, "SOLUTION +1 TENT de devrait pas etre gagnant\n");
    game_delete(sol);
    game_delete(def);
    return false;
  }
  game sol1=game_default_solution();
  game_set_square(sol1,7,7,GRASS);
  if(game_won(sol1)){
    fprintf(stderr, "SOLUTION +1 GRASS ne devrait pas etre gagnant\n");
    game_delete(sol);
    game_delete(sol1);
    game_delete(def);
    return false;
  }
  game_delete(sol);
  game_delete(sol1);
  game_delete(def);

  game sol2=game_default_solution();
  uint nb_trees=0;
  for (uint i=0; i<DEFAULT_SIZE; i++){
    for (uint j=0; j<DEFAULT_SIZE; j++){
      if (game_get_square(sol2,i,j)==TENT){
        if (!game_is_neigh(sol2,i,j,TREE,false)){
          game_delete(sol2);
          return false;
        }
        if (game_is_neigh(sol2,i,j,TENT,true)){
          game_delete(sol2);
          return false;
        }
      }
      if (game_get_square(sol2,i,j)==TREE){
        nb_trees++;
      }
    }
  }
  if (game_get_expected_nb_tents_all(sol2) != nb_trees){
    game_delete(sol2);
    return false;
  }

  uint nb_tents_col=0;
  uint nb_tents_row=0;
  for (uint i=0; i<DEFAULT_SIZE; i++){
    nb_tents_row=0;
    for (uint j=0; j<DEFAULT_SIZE; j++){
      if (game_get_square(sol2,i,j)==TENT){
        nb_tents_row++;
      }
    }
    if (game_get_current_nb_tents_row(sol2, i)!=nb_tents_row){
      game_delete(sol2);
      return false;
    }
  }
  for (uint j=0; j<DEFAULT_SIZE; j++){
    nb_tents_col=0;
    for (uint i=0; i<DEFAULT_SIZE; i++){
      if (game_get_square(sol2,i,j)==TENT){
        nb_tents_col++;
      }
    }
    if (game_get_current_nb_tents_col(sol2, j)!=nb_tents_col){
      game_delete(sol2);
      return false;
    }
  }
  game_delete(sol2);
  return true;
  }

bool test_game_check_move(){
  game g=game_new_empty();
  game_set_square(g,2,2,TREE);

  if(!game_check_move(g,0,0,TENT)){
    fprintf(stderr, "poser une TENT sur une case vide devrait etre legal\n");
    game_delete(g);
    return false;
  }
  if(game_check_move(g,2,2,TENT)){
    fprintf(stderr, "on devrait pas pouvoir modifier une case TREE\n");
    game_delete(g);
    return false;
  }
  if(game_check_move(g,1,1,TREE)){
    fprintf(stderr, "on devrait pas pouvoir ajouter une case TREE\n");
    game_delete(g);
    return false;
  }
  if(game_check_move(g,8,8,TENT)){
    fprintf(stderr, "on ne devrait pas pouvoir poser une case en dehors des limite de la grille\n");
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}




void usage(int argc, char *argv[])
{
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
  if (argc == 1)
    usage(argc, argv);

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy",argv[1])==0)
    ok = test_dummy();
  else if (strcmp("game_is_neigh",argv[1])==0)
    ok = test_game_is_neigh();
  else if (strcmp("game_get_ajacent_square",argv[1])==0)
    ok = test_game_get_ajacent_square();
  else if (strcmp("game_default_solution",argv[1])==0)
    ok = test_game_default_solution();
  else if (strcmp("game_default",argv[1])==0)
    ok = test_game_default();
  else if (strcmp("game_restart",argv[1])==0)
    ok = test_game_restart();
  else if (strcmp("game_fill_grass",argv[1])==0)
    ok = test_game_fill_grass();
  else if (strcmp("game_won",argv[1])==0)
    ok = test_game_won();
  else if (strcmp("game_check_move",argv[1])==0)
    ok = test_game_check_move();
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