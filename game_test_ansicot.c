#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_dummy(){
    return true;
}

int main(int argc, char *argv[]){
    bool ok = false;
    if (strcmp("dummy",argv[1])==0){
        ok = test_dummy();
    }
    else{
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        exit(EXIT_FAILURE);
    } 

    if (ok){
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }
    else{
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}