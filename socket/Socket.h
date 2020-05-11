//
// Created by lothar on 26/03/2020.
//

#ifndef C_HAT_SOCKET_H
#define C_HAT_SOCKET_H


#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>

#include "../log/Logger.h"
#include "../client/Client.h"
#include "../room/Room.h"
#include "../message manager/Message Manager.h"

//Listening Port
#define PORT 1234

//Listening adress (all the addresses)
#define IP INADDR_ANY


//Size of the waiting list
#define BACKLOG 3

//Size of the buffer
#define BUFFER_LEN 10


//Socket descriptor
int fd_server;

//Structure holding the adress
struct sockaddr_in adress_server;

void accept_client();

void *manage_client(void *arg);

void start_socket();


void send_self_msg(client *ptr_client, char *format, ...);
//Structure holding our clients
#define CLIENTS_SIZE 10
client clients[CLIENTS_SIZE]; //object collection or struct collection is written with an "s" in prog



#endif //C_HAT_SOCKET_H