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
        fprintf(stderr, "La fonction renvoie un pointeur NULL\n");
        free(squares);
        return false;
    }
    game g2 = game_new_empty();
        if (g2 == NULL) {
        fprintf(stderr, "La fonction game_new_empty() a renvoye un pointeur NULL\n");
        game_delete(g);
        free(squares);
        return false;
    }
    if (!game_equal(g, g2)){ 
        game_delete(g);
        game_delete(g2);
        free(squares);
        return false;
    }
    game_print(g);
    game_delete(g);
    game_delete(g2);
    free(squares);
    return true;
}

bool test_new_empty() {
    game g = game_new_empty();
    if (g == NULL) {
        fprintf(stderr, "La fonction renvoie un pointeur NULL\n");
        return false;
    }
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        for (uint j = 0; j < DEFAULT_SIZE; j++) {
            if (game_get_square(g, i, j) != EMPTY) {
                fprintf(stderr, "La case (%u,%u) n'est pas vide\n", i, j);
                game_delete(g);
                return false;
            }
        }
    }
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
        if (game_get_expected_nb_tents_row(g, i) != 0) {
            fprintf(stderr, "Le nombre de tentes attendu sur la ligne [%u] n'est pas de 0\n", i);
            game_delete(g);
            return false;
        }
    }
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
        if (game_get_expected_nb_tents_col(g, j) != 0) {
            fprintf(stderr, "Le nombre de tentes attendu sur la colonne [%u] n'est pas de 0\n", j);
            game_delete(g);
            return false;
        }
    }
    game_print(g);
    game_delete(g);
    return true;
}

bool test_copy() {
    game g1 = game_default();
    if (g1 == NULL){
        return false; 
    }
    game g2 = game_copy(g1);
    if (g2 == NULL) {
        fprintf(stderr, "La fonction renvoie un pointeur NULL\n");
        game_delete(g1);
        return false;
    }
    if (!game_equal(g1, g2)) {
        fprintf(stderr, "La copie n'est pas egale a l'originale\n");
        game_delete(g1);
        game_delete(g2);
        return false;
    }
    game_set_square(g2, 1, 0, TENT);
    if (game_get_square(g1, 1, 0) == TENT) {
        fprintf(stderr, "La copie modifie aussi l'originale\n");
        game_delete(g1);
        game_delete(g2);
        return false;
    }
    game_print(g1);
    game_print(g2);
    game_delete(g1);
    game_delete(g2);
    return true;
}

bool test_equal() {
    game g1 = game_default();
    if (g1 == NULL){
        return false;
    }
    game g2 = game_copy(g1);
    if (g2 == NULL){
        game_delete(g1);
        return false;
    }
    if (!game_equal(g1, g2)) {
        fprintf(stderr, "Les deux games devraient etres egales a la creation\n");
        game_delete(g1);
        game_delete(g2);
        return false;
    }
    game_set_square(g2, 1, 0, TENT);
    if (game_equal(g1, g2)) {
        fprintf(stderr, "Les deux games devraient etres differentes des qu'on en modifie une\n");
        game_delete(g1);
        game_delete(g2);
        return false;
    }
    game_set_square(g2, 1, 0, EMPTY);
    game_print(g1);
    game_print(g2);
    game_delete(g1);
    game_delete(g2);
    return true;
}

bool test_set_square() {
    game g1 = game_default();
    if (g1 == NULL){
        return false;
    }
    game g2 = game_copy(g1);
    if (g2 == NULL){
        game_delete(g1);
        return false;
    }
    if (game_get_square(g1, 1, 0) != EMPTY) {
        fprintf(stderr, "La case a mal ete initialisee\n");
        game_delete(g1);
        game_delete(g2);
        return false;
    }
    game_set_square(g1, 1, 0, TENT);
    if (game_get_square(g1, 1, 0) != TENT) {
        fprintf(stderr, "La case n'a ete correctement remplie\n");
        game_delete(g1);
        game_delete(g2);
        return false;
    }
    game_set_square(g1, 1, 0, EMPTY);
    if (!game_equal(g1, g2)) {
        fprintf(stderr, "set_square a aussi modifiee d'autres case que celle voulue\n");
        game_delete(g1);
        game_delete(g2);
        return false;
    }
    game_print(g1);
    game_print(g2);
    game_delete(g1);
    game_delete(g2);
    return true;
}

bool test_get_square() {
    game g1 = game_default();
    if (g1 == NULL){
        return false;
    }
    if (game_get_square(g1, 1, 0) != EMPTY) {
        fprintf(stderr, "On ne trouve pas la bonne valeur pour cette case\n");
        game_delete(g1);
        return false;
    }
    game_set_square(g1, 1, 0, TENT);
    if (game_get_square(g1, 1, 0) != TENT) {
        fprintf(stderr, "On ne trouve pas la bonne valeur pour cette case\n");
        game_delete(g1);
        return false;
    }
    game_set_square(g1, 1, 0, EMPTY);
    game_print(g1);
    game_delete(g1);
    return true;
}

/* ********** USAGE ********** */

void usage(int argc, char *argv[]){
    fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
    exit(EXIT_FAILURE);
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[]){
    if (argc == 1)
        usage(argc, argv);
    // start test
    fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
    bool ok = false;
    if (strcmp("dummy", argv[1]) == 0)
        ok = test_dummy();
    else if (strcmp("new", argv[1]) == 0)
        ok = test_new();
    else if (strcmp("new_empty", argv[1]) == 0)
        ok = test_new_empty();
    else if (strcmp("copy", argv[1]) == 0)
        ok = test_copy();
    else if (strcmp("equal", argv[1]) == 0)
        ok = test_equal();
    else if (strcmp("set_square", argv[1]) == 0)
        ok = test_set_square();
    else if (strcmp("get_square", argv[1]) == 0)
        ok = test_get_square();

    else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
    }
    // print test result
    if (ok){
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}
