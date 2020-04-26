//
// Created by user on 24/03/2020.
//

#include "bindParam.h"

int bindParam(int fdSock4){
    int option = 1;
    if (setsockopt(fdSock4, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) != 0) {
        printf("Bind Param Error : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}