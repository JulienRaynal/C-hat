//
// Created by kya on 17/03/2020.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BACKLOG 4

int socketCreation (){
    int fdSock4 = socket(AF_INET, SOCK_STREAM, 0);
    if (fdSock4 == -1) {
        printf("Creation Failed. Error Code : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fdSock4;
}

int bindParam(int fdSock4){
    int option = 1;
    if (setsockopt(fdSock4, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) != 0) {
        printf("Bind Param Error : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

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

int listening(int fdSock4){
    if(listen(fdSock4, BACKLOG) != 0 ){
        printf("Failed to listen : %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
}

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