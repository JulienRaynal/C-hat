//
// Created by user on 24/03/2020.
//
#include "creation.h"

int socketCreation (){
    int fdSock4 = socket(AF_INET, SOCK_STREAM, 0);
    if (fdSock4 == -1) {
        printf("Creation Failed. Error Code : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fdSock4;
}