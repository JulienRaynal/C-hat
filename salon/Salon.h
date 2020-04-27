//
// Created by kya on 26/04/2020.
//

#ifndef C_HAT_SALON_H
#define C_HAT_SALON_H

#include "../client/Client.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct room {
    char *name;
    int nbClient;
}room;

void room_init(room *r);

void room_destroy(room *r);

int room_set_name(room *r, const char *name);

int is_created(room *r);

#endif //C_HAT_SALON_H
