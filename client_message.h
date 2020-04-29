#ifndef __CLIENT_MESSAGE_H__
#define __CLIENT_MESSAGE_H__

typedef struct client_message client_message_t;

#include "msgbuffer.h"

typedef struct client_message {
    char* message;
    size_t msgmemory;
    size_t msglenght;
    msgbuffer_t msgbuffer;
} client_message_t;

int client_message_create(client_message_t *self, FILE *input);

int client_message_destroy(client_message_t *self);

int client_message_realloc(client_message_t *self);

int client_message_to_DBUS(client_message_t *self, size_t msg_id);

#endif