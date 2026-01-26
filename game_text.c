#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char* argv[]) { 
  char c;
  int i;
  int j;
  char name[50];
  game g = NULL;
  if (argc > 1){
    g = game_load(argv[1]);
  } else{
    g = game_default();
  }
  while (!game_won(g)) {
    game_print(g);
    printf("? [h pour aide]\n");
    scanf(" %c", &c);
    if (c == 'h') {
      printf(
          "Si 't', met une pièce TENT dans la case (<i>,<j>)\nSi 'g', met la pièce GRASS dans la case (<i>,<j>)\nSi "
          "'e', met la pièce EMPTY dans la case (<i>,<j>)\nSi 'f', remplie la grille avec de l'herbe (fill grass)\nSi "
          "'z', annule le dernier coup (undo)\nSi 'y', refait le dernier coup annulé (redo)\nSi "
          "'r', le jeu redémarre\nSi 's <filename>', sauve le jeu en cours\nSi 'q', quitter le jeu\n");
    } else if (c == 't' || c == 'g' || c == 'e') {
      scanf(" %d %d", &i, &j);
      if (c == 't') game_play_move(g, i, j, TENT);
      if (c == 'g') game_play_move(g, i, j, GRASS);
      if (c == 'e') game_play_move(g, i, j, EMPTY);
    } else if (c == 'r')
      game_restart(g);
    else if (c == 'f')
      game_fill_grass(g);
    else if (c == 'z')
      game_undo(g);
    else if (c == 'y')
      game_redo(g);
    else if (c=='s'){
      scanf(" %s", name);
      game_save(g, name);
    }
    else if (c == 'q') {
      game_delete(g);
      printf("shame\n");
      return EXIT_SUCCESS;
    }
  }
  game_print(g);
  printf("congratulation\n");
  game_delete(g);
  return EXIT_SUCCESS;
}