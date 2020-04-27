//
// Created by kya on 26/04/2020.
//

#include "Salon.h"

void room_init(room *r) {
    r->name = NULL;
    r->nbClient = 0;
}

void room_destroy(room *r) {
    free(r->name);
    room_init(r);
}

int room_set_name(room *r, const char *name) {
    int len = strlen(name);//length of our name
    char *tmp; //we create a temporary value to store our data and not loose c->name

    if (r->name == NULL) {
        tmp = calloc(len, sizeof(char)); //We want the size of the length of the name time the size of a char
    } else {
        tmp = realloc(r->name, len * sizeof(char)); //We realloc the name variable with a new size
    }
    if (tmp == NULL) {
        return EXIT_FAILURE;
    }
    r->name = tmp;
//this function needs a const;  "const" says we won't change the character chain we only are able to read it and not set
    strcpy(r->name, name);
    return EXIT_SUCCESS;
}

int is_created(room *r) {
    return r->name == NULL ? EXIT_SUCCESS : EXIT_FAILURE; //faster than if else
}