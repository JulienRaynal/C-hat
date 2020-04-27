//
// Created by lothar on 26/03/2020.
//

#ifndef C_HAT_CLIENT_H
#define C_HAT_CLIENT_H
#define FREE_VALUE -1

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>



//Our client structure
typedef struct client{
    int socket;
    char *pseudo; //use calloc/malloc to assign the size of the name  "*" means their will be a dynamic space allocation
    //creating thread for each connexion but they can't talk to each other because they're not in the same stack
    pthread_t thread;
    int roomNumber;
} client;

void client_init(client *c); //"*" to avoid copies and free it properly
int set_pseudo(client *c, const char *pseudo);
void client_destroy(client *c);
int is_free(client *c);

//backlog = number that are in front of the door NOT the ones in it

#endif //C_HAT_CLIENT_H