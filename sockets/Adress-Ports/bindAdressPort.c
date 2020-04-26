//
// Created by user on 24/03/2020.
//

#include "bindAdressPort.h"

int binAdressPort(int fdSock4){
    struct sockaddr_in adress;
    adress.sin_family = AF_INET;
    adress.sin_addr.s_addr = INADDR_ANY;
    adress.sin_port = htons(42);

    if(bind(fdSock4, (struct sockaddr *) &adress, sizeof(adress)) != 0){
        printf("Failed to bind : %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
}