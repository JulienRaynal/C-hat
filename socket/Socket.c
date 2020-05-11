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
    send(fd_client, "Welcome in C-HAT - If you need help use !help\nYour next message will be your pseudo\n",
         sizeof("Welcome in C-HAT - If you need help use !help\nYour next message will be your pseudo\n"),
         MSG_DONTWAIT);

    //we define the length of the buffer and add +1 for \0  //"*" allows to not give a size in opposite of []

    char *msg = NULL; //clearing the value of msg to have it empty and not giving a length
    char buffer[BUFFER_LEN + 1];
    int msgLen = 0;
    int changeName = 2, changeRoom = 1;

    while (1) {
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

        if (changeName == 0 || changeName == 2) {
            msg[msgLen - 2] = '\0';
            set_pseudo(ptr_client, msg);
            if (changeName == 2) {
                send_msg(ptr_client, "%s is connected\n", ptr_client->pseudo);
            }
            changeName = 1;

        } else if (changeRoom == 0) {
            for (int i = 0; i < ROOM_MAX; i++) {
                if (rooms[i].name == NULL) {
                    msg[msgLen - 2] = '\0'; //We put the \0 earlier because we don't want the auto \n
                    room_initiate_room(&rooms[i], msg, ptr_client, i);
                    send(fd_client, "Room successfully created\n",
                         sizeof("Room successfully created\n"), MSG_DONTWAIT);
                    changeRoom = 1;
                    break;
                }
            }
        } else if (strncmp(msg, "!help", strlen("!help")) == 0) {
            display_commands(ptr_client);

        } else if (strncmp(msg, "!nick", strlen("!nick")) == 0) {
            send(fd_client, "Your next message will be your nickname\n",
                 sizeof("Your next message will be your nickname\n"), MSG_DONTWAIT);
            changeName = 0;

        } else if (strncmp(msg, "!users", strlen("!users")) == 0) {
            for (int i = 0; i < CLIENTS_SIZE; i++) {
                if (clients[i].socket != ptr_client->socket && clients[i].roomNumber == ptr_client->roomNumber &&
                    is_free(&clients[i]) == EXIT_FAILURE) {
                    send_self_msg(ptr_client, "[%s]\n", clients[i].pseudo);
                }
            }

        } else if (strncmp(msg, "!room", strlen("!room")) == 0) {
            int a = strtol(msg + 6, NULL, 10);
            if (a < 0 || a > ROOM_MAX) {
                send(fd_client, "You didn't enter a good Room number\n",
                     sizeof("You didn't enter a good Room number\n"), MSG_DONTWAIT);
            } else {
                ptr_client->roomNumber = a;
                send_msg(ptr_client, "%s has joined the Room\n", ptr_client->pseudo);
                display_room_name(ptr_client);
                display_Room_Message(ptr_client);
            }
            //TODO vérifier erreur si on rentre rien
            //compare that the msgs equals "!exit" and the compares from [0] to the length of "!exit"
        } else if (strncmp(msg, "!create", strlen("!create")) == 0) {
            for (int i = 0; i < ROOM_MAX; i++) {
                if (rooms[i].name != NULL) {
                    continue;
                } else {
                    send(fd_client, "Your next message will define the room name\n",
                         sizeof("Your next message will define the room name\n"), MSG_DONTWAIT);
                    changeRoom = 0;
                    break;
                }
                send(fd_client, "All the rooms are already taken\n", sizeof("All the rooms are already taken\n"),
                     MSG_DONTWAIT);
                //TODO afficher le msg si aucune room est dispo
            }
        } else if (strncmp(msg, "!exit", strlen("!exit")) == 0) {
            log_msg("The user : %s has left\n", ptr_client->pseudo);
            send_msg(ptr_client, "%s has left\n", ptr_client->pseudo);

            free(msg);
            msg = NULL;
            msgLen = 0;
            break;
            //else we send the message somewhere else
        } else {
            send_msg(ptr_client, "%s : %s", ptr_client->pseudo, msg);
        }
        //We reset our parameters
        free(msg);
        msg = NULL;
        msgLen = 0;
    }
    int roomToDestroy = ptr_client->roomNumber;
    client_destroy(ptr_client);
    close(fd_client);
    for (int i = 1; i < CLIENTS_SIZE; i++) { // 1 because we want to keep the room 0 always available
        if (clients[i].roomNumber == roomToDestroy) {
            break;
        } else {
            room_destroy(&rooms[i]);
        }
    }
    //we close the socket of the form data of the client
    pthread_exit(EXIT_SUCCESS);
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
