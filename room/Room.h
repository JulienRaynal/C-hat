//
// Created by kya on 26/04/2020.
//

#ifndef C_HAT_ROOM_H
#define C_HAT_ROOM_H


#include <stddef.h>
#include "../socket/Socket.h"
#include "../client/Client.h"

typedef struct room {
    char *name;
    char *room_message;
}room;

void room_init(room *r);

void room_destroy(room *r);

void display_room_name(client *c);

int room_initiate_room(room *r, const char *name, client *ptr_client, int room_number);

void display_Room_Message(client *c);

void notification_message(client *ptr_client, char *format, ...);
//Structure holding our rooms

#define ROOM_MAX 5
room rooms[ROOM_MAX];



#endif //C_HAT_ROOM_H
