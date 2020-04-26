//
// Created by user on 24/03/2020.
//

#include "connexion.h"

int connexion(int fdSock4){
    fcntl(fdSock4,F_SETFL, O_NONBLOCK);

    //Connexion
    struct sockaddr_in clientAdress;
    unsigned int addrLen = sizeof(clientAdress);
    int clientSocket = accept(fdSock4,(struct sockaddr *) &clientAdress, &addrLen);
    if (clientSocket != -1){
        //IP Conversion
        char ip [INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAdress.sin_addr), ip, INET_ADDRSTRLEN);
        printf("Connexion from %s:%i\n",ip,clientAdress.sin_port);
    }
}