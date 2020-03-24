//
// Created by user on 24/03/2020.
//

#include "listening.h"

int listening(int fdSock4){
    if(listen(fdSock4, BACKLOG) != 0 ){
        printf("Failed to listen : %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
}