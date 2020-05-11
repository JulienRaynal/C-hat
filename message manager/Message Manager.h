//
// Created by kya on 05/05/2020.
//

#ifndef C_HAT_MESSAGE_MANAGER_H
#define C_HAT_MESSAGE_MANAGER_H


#include "../socket/Socket.h"
#include "../client/Client.h"


void send_msg(client *ptr_client, char *format, ...);

void send_self_msg(client *ptr_client, char *format, ...);

void display_commands();

#endif //C_HAT_MESSAGE_MANAGER_H
