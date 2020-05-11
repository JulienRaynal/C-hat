//
// Created by kya on 26/04/2020.
//

#include "Room.h"

void room_init(room *r) {
    r->name = NULL;
}

void room_destroy(room *r) {
    room_init(r);
    free(r->name);
}

int room_initiate_room(room *r, const char *name, client *ptr_client, int room_number) {
    int len = strlen(name);//length of our name
    char *tmp; //we create a temporary value to store our data and not loose c->name

    if (r->name == NULL) {
        tmp = calloc(len, sizeof(char)); //We want the size of the length of the name time the size of a char
    } else {
        tmp = realloc(r->name, len * sizeof(char)); //We realloc the name variable with a new size
    }
    if (tmp == NULL) {
        return EXIT_FAILURE;
    }
    r->name = tmp;
//this function needs a const;  "const" says we won't change the character chain we only are able to read it and not set
    strcpy(r->name, name);
    notification_message(ptr_client, "Your room number is [%i]\n", room_number);
    return EXIT_SUCCESS;
}

void display_Room_Message(client *c) {
    rooms[0].room_message = "Welcome to the Room 0\n";
    client *ptr_client = c; //pointer to our client
    int room_number = c->roomNumber; //takes the value of our actual room
    for (int i =0; i < ROOM_MAX; i++){
        if (i != room_number || rooms[i].room_message == NULL){ //doesn't send the message to all rooms
            break;
        } else {
            notification_message(ptr_client, "%s\n", rooms[i].room_message); //sends the room custom message
            for (int j=0; j<CLIENTS_SIZE; j++){
                if (clients[j].socket != ptr_client->socket && is_free(&clients[j]) == EXIT_FAILURE && ptr_client->roomNumber == clients[j].roomNumber){ //checks that we send the message to the client and not all the room
                    notification_message(ptr_client, "[%s] : is connected\n", clients[i].pseudo);
                }
            }
        }
    }
}

void display_room_name(client *c) {
    client *ptr_client = c;
    int room_number = c->roomNumber;
    for (int i = 0; i < ROOM_MAX; i++) {
        if (i != room_number || rooms[i].name == NULL) {
            continue;
        } else {
            notification_message(ptr_client, "Welcome to the room [%s]\n", rooms[i].name);
        }
    }
}


void notification_message(client *ptr_client, char *format, ...) {
    va_list params; //taking all the other params
    va_start(params, format); //we start read the params needed
    int len = vsnprintf(0, 0, format, params); //we calculate the length by looking at the format and the length of our params (prt_client->pseudo & msg)
    char tmp[len+1]; //+1 for the '\0'
    va_start(params, format);
    vsprintf(tmp, format, params); //we finally put our message in tmp
    va_end(params);
    for (int i = 0; i < CLIENTS_SIZE; ++i) {
        if (clients[i].socket == ptr_client->socket) {
            send(clients[i].socket, tmp, len, MSG_DONTWAIT);
        }
    }
}
