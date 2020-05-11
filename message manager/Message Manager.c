//
// Created by kya on 05/05/2020.
//

#include "Message Manager.h"

void send_msg(client *ptr_client, char *format, ...) {
    va_list params; //taking all the other params
    va_start(params, format); //we start read the params needed
    int len = vsnprintf(0, 0, format,
                        params); //we calculate the length by looking at the format and the length of our params (prt_client->pseudo & msg)
    char tmp[len + 1]; //+1 for the '\0'
    va_start(params, format);
    vsprintf(tmp, format, params); //we finally put our message in tmp
    va_end(params);
    for (int i = 0; i < CLIENTS_SIZE; ++i) {
        if (clients[i].socket != ptr_client->socket && is_free(&clients[i]) == EXIT_FAILURE &&
            ptr_client->roomNumber == clients[i].roomNumber) {
            send(clients[i].socket, tmp, len, MSG_DONTWAIT);
        }
    }
}

void send_self_msg(client *ptr_client, char *format, ...) {
    va_list params; //taking all the other params
    va_start(params, format); //we start read the params needed
    int len = vsnprintf(0, 0, format,
                        params); //we calculate the length by looking at the format and the length of our params (prt_client->pseudo & msg)
    char tmp[len + 1]; //+1 for the '\0'
    va_start(params, format);
    vsprintf(tmp, format, params); //we finally put our message in tmp
    va_end(params);
    send(ptr_client->socket, tmp, len, MSG_DONTWAIT);
}

void display_commands(client *ptr_client) {
    send_self_msg(ptr_client, "%s : %s\n", "!help ", " see other commands");
    send_self_msg(ptr_client, "%s : %s\n", "!nick ", " change your nickname");
    send_self_msg(ptr_client, "%s : %s\n", "!users ", " see who you’re talking to");
    send_self_msg(ptr_client, "%s : %s\n", "!room [room number]  ", " change your room");
    send_self_msg(ptr_client, "%s : %s\n", "!create ", " create a new room");
    send_self_msg(ptr_client, "%s : %s\n", "!exit ", " leave the server");
}
