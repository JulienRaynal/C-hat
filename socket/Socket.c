//
// Created by lothar on 26/03/2020.
//

#include "Socket.h"

void accept_client() {
    int clientSocket;
    //Structure containing our client structure
    struct sockaddr_in clientAddress; //creature a structure that will contain the client address
    int addrLen = sizeof(clientAddress); //creates a variable containing the size of the client address
    // We connect/put the parameters of the socket and the length of the socket and check it doesn't return an error that is -1 with this function
    if ((clientSocket = accept(fd_server, (struct sockaddr *) &clientAddress, (socklen_t *) &addrLen)) != -1) {
        //convert the ip in text
        char ip[INET_ADDRSTRLEN]; //makes a table that takes the maximal length of an ipv4 address
        //converts ipv4 binary address in text; AF_INET designate the type of addresses the socket can communicate with (ipv4)
        inet_ntop(AF_INET, &(clientAddress.sin_addr), ip, INET_ADDRSTRLEN);
        //sin_addr is the client ip
        //copies the client ip to the ip variable
        //INET ADDRSTRLEN specifies the size of the variable ip

        client *free_client = NULL; //"*" because it's a pointer

        for (int i = 0; i < CLIENTS_SIZE; i++) {
            if (is_free(&clients[i]) == EXIT_SUCCESS) { //we need the pointer not the object
                free_client = &clients[i]; //"&" I don't need the object but the memory address
                break;
            }
        }
        if (free_client == NULL) {
            send(clientSocket, "Server already at full capacity\n", sizeof("Server already at full capacity\n"),
                 MSG_DONTWAIT);
            close(clientSocket);
            return;
        }

        log_msg("Connexion from %s:%i", ip, clientAddress.sin_port); //print the connexion info

        free_client->socket = clientSocket;

        //We use the old parameters of manage_client
        pthread_create(&free_client->thread, NULL, manage_client, free_client);
    }
}

void *manage_client(void *arg) { //We put the file descriptor of the client and his socket has parameters
    client *ptr_client = (client *) arg; //cast = force the compiler to take a client*
    int fd_client = ptr_client->socket;
    //sends to the socket of the client the message and doesn't wait for the confirmation of reception
    send(fd_client, "Welcome in C-HAT\nYour next message will be your pseudo\n",
         sizeof("Welcome in C-HAT\nYour next message will be your pseudo\n"), MSG_DONTWAIT);
    //we define the length of the buffer and add +1 for \0  //"*" allows to not give a size in opposite of []
    char buffer[BUFFER_LEN + 1];
    char *msg = NULL; //clearing the value of msg to have it empty and not giving a length
    int msgLen = 0, pMsgLen;
    int changeName = 0, changeRoom = 1;

    while (1) {
        //gives the length of the msg received from a socket //if the msg is too long to hold in the buffer the missing part will be deleted
        int len = recv(fd_client, &buffer, BUFFER_LEN, 0);
        buffer[len] = '\0'; //we add at the end of the buffer a '\0' to signal the end the message

        if (len == -1 && errno != EAGAIN) {//Checks that we have an error and the call isn't stuck
            //An error happened
            log_msg("Errno [%i] : %s\n", errno, strerror(errno)); //errno is the last error number
            break;
        } else if (len == 0) {
            //if the client disconnected (socket closed)
            log_msg("The socket has been closed");
            break;
        }
        msgLen += len; //adds the len of the msg received from a socket
        if (msg == NULL) {
            //if our message is empty we give him the space necessary (length of the message * the size of a char)
            msg = calloc(len, sizeof(char));
        } else {
            //if our message isn't empty we add more space until we get the whole message
            msg = realloc(msg, msgLen * (sizeof(char)));
        }
        strcat(msg, buffer); //appends the message from buffer and puts in msg
        if ('\r' != msg[msgLen - 2]) { //We check if we finished to get all the message from the buffer
            continue; //continue leaves the loop
        }
        msg[msgLen - 2] = '\n'; //we put a /n at the end of our message
        msg[msgLen - 1] = '\0'; //We signal that this is the end of the message
        log_msg("Receive from the socket %i :\n %s : %s", fd_client, ptr_client->pseudo, msg);

        if (changeName == 0) {
            msg[msgLen - 2] = '\0';
            set_pseudo(ptr_client, msg);
            changeName = 1;

        } else if (strncmp(msg, "!nick", strlen("!nick")) == 0) {
            send(ptr_client->socket, "Your next message will be your pseudo\n",
                 sizeof("Your next message will be your pseudo\n"), MSG_DONTWAIT);
            changeName = 0;

        } else if (strncmp(msg, "!room", strlen("!room")) == 0) {
            int a = strtol(msg + 6, NULL, 10);
            if (a < 0 || a > ROOM_MAX) {
                send(ptr_client->socket, "You didn't enter a good room number\n",
                     sizeof("You didn't enter a good room number\n"), MSG_DONTWAIT);
            } else {
                ptr_client->roomNumber = a;
                send_msg(ptr_client, "%s joined the room\n", ptr_client->pseudo);
            }
            //compare that the msgs equals "!exit" and the compares from [0] to the length of "!exit"
        } else if (strncmp(msg, "!exit", strlen("!exit")) == 0) {
            log_msg("The user : %s has left\n", ptr_client->pseudo);
            for (int i = 0; i < CLIENTS_SIZE; ++i) {
                if (clients[i].socket != fd_client && is_free(&clients[i]) == EXIT_FAILURE) {
                    send_msg(ptr_client, "The user %s has left\n", ptr_client->pseudo);
                }
            }
            free(msg);
            msg = NULL;
            msgLen = 0;
            log_msg("Message freed");
            break;
            //else we send the message somewhere else
        } else {
            send_msg(ptr_client, "%s : %s\n", ptr_client->pseudo, msg);
        }
        //We reset our parameters
        free(msg);
        msg = NULL;
        msgLen = 0;
    }
    client_destroy(ptr_client);
    close(fd_client); //we close the socket of the form data of the client
    pthread_exit(EXIT_SUCCESS);
}

void send_msg(client *ptr_client, char *format, ...) {
    va_list params;
    va_start(params, format);
    int len = vsnprintf(0, 0, format, params);
    char tmp[len + 1];
    va_start(params, format);
    vsprintf(tmp, format, params);
    va_end(params);
    for (int i = 0; i < CLIENTS_SIZE; ++i) {
        if (clients[i].socket != ptr_client->socket && is_free(&clients[i]) == EXIT_FAILURE &&
            ptr_client->roomNumber == clients[i].roomNumber) {
            send(clients[i].socket, tmp, len, MSG_DONTWAIT);
        }
    }
}

void start_socket() {
    adress_server.sin_family = AF_INET;
    adress_server.sin_addr.s_addr = IP;
    adress_server.sin_port = htons(PORT);
// Nombre d'options
    int opt = 1;
// Création de la socket en TCP
    if ((fd_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        log_msg("Echéc de la création: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    log_msg("Création de la socket\n");
// Paramètrage de la socket
    if (setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)) != 0) {
        log_msg("Echéc de paramètrage: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    log_msg("Paramètrage de la socket\n");
// Attachement de la socket sur le port et l'adresse IP
    if (bind(fd_server, (struct sockaddr *) &adress_server, sizeof(adress_server)) != 0) {
        log_msg("Echéc d'attachement: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    log_msg("Attachement de la socket sur le port %i\n", PORT);
// Passage en écoute de la socket
    if (listen(fd_server, BACKLOG) != 0) {
        log_msg("Echéc de la mise en écoute: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
// Passage en mode non bloquant
//    fcntl(fd_server, F_SETFL, O_NONBLOCK);
    log_msg("Mise en écoute de la socket\n");
}
