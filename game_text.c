#include <stdlib.h>
#include "game.h"
#include "game_aux.h"

int main(void){
    game g = game_default();
    while (!game_won(g)){
        game_print(g);
        char c;
        char ret = scanf("%c",&c);
        if ('c'=='h'){
            "afficher l'aide"
        }
        else if ('c'=='t' || 'c'=='g' || 'c'=='e') {
            "Lire deux entiers <i> <j> sur l'entrée standard (au format "%d %d")"
            "Si <c> == 't', mettre une pièce TENT dans la case (<i>,<j>)
            Si <c> == 'g', mettre la pièce GRASS dans la case (<i>,<j>)
            Si <c> == 'e', mettre la pièce EMPTY dans la case (<i>,<j>)"
        }
        else if ('c'=='r'){
            game_restart(g);
        }
        else if  ('c'=='f'){
            game_fill_grass(g);
        }
        else if ('c'=='q'){
            "quitter"
        }
    }
    game_print(g);



    game g = game_default();
    game_play_move(g,1,0,TENT);
    game_print(g);
    game_delete(g);
    return EXIT_SUCCESS;
}