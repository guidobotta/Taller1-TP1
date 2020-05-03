#ifndef __CLIENT_MSGBUFFER_H__
#define __CLIENT_MSGBUFFER_H__

#include "client_message.h"
#include <stdio.h>

typedef struct client_msgbuffer {
    char buffer[32];
} client_msgbuffer_t;

int client_msgbuffer_create(client_msgbuffer_t *self);

int client_msgbuffer_destroy(client_msgbuffer_t *self);

int client_msgbuffer_getline(client_msgbuffer_t *self, client_message_t *msg, FILE *input);

#endif