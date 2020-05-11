#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "socket/Socket.h"
#include "log/Logger.h"

//TODO enlever les values grisé inutiles pour clean de .c et .h
//TODO vérifier que ça marche bien à 2

void signal_handler(int sig);

void daemonize();

int end = 0;

int main(void) {
    daemonize();
    open_log_file();
    start_socket();

    //initializing our clients with pseudo = NULL and socket = FREE VALUE
    for(int i=0; i<CLIENTS_SIZE; i++){
        client_init(&clients[i]); //we need the pointer not the object
    }
    for(int i=0; i<ROOM_MAX;i++){
        room_init(&rooms[i]);
    }
    rooms[0].name= "Home";
    while (end == 0) {
        accept_client();
    }
    for (int j = 0; j < ROOM_MAX ; ++j) {
        room_destroy(&rooms[j]);
    }
    return EXIT_SUCCESS;
}

void daemonize() {
    //daemon(0, 0);
    signal(2, signal_handler);
    signal(15, signal_handler);
}

void signal_handler(int sig) {
    printf("We receive %i\n", sig);
    end = 1;
}