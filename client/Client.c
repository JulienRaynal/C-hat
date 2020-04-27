//
// Created by lothar on 26/03/2020.
//

#include "Client.h"

void client_init(client *c){
    c->socket = FREE_VALUE; //in the stack doesn't need to be freed
    c->pseudo = NULL; //in the heap needs to be freed (calloc/realloc)
    c->roomNumber = 0;
}

void client_destroy(client *c){
    free(c->pseudo); //only memory address given through calloc/malloc/realloc
    //c->pseudo = NULL;//still was the old memory address, needs to be reset (the memory space doesn't belong to him anymore)
    client_init(c);
}

int set_pseudo(client *c, const char *pseudo){
    int len = strlen(pseudo);//length of our pseudo
    char *tmp; //we create a temporary value to store our data and not loose c->pseudo

    if (c->pseudo == NULL){
        tmp = calloc(len, sizeof(char)); //We want the size of the length of the pseudo time the size of a char
    } else {
        tmp = realloc(c->pseudo,len * sizeof(char)); //We realloc the pseudo variable with a new size
    }

    if (tmp == NULL){
        return EXIT_FAILURE;
    }
    c->pseudo = tmp;

    strcpy(c->pseudo, pseudo); //this function needs a const;  "const" says we won't change the character chain we only are able to read it and not set

    return EXIT_SUCCESS;
}

int is_free(client *c){
    return c->socket == FREE_VALUE ? EXIT_SUCCESS : EXIT_FAILURE; //faster than if else
}