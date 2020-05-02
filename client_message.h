#ifndef __CLIENT_MESSAGE_H__
#define __CLIENT_MESSAGE_H__

typedef struct client_message client_message_t;

#include "msgbuffer.h"
#include "info_client.h"
#include <stdint.h>

typedef struct client_message {
    char* message;
    uint32_t msgmemory;
    uint32_t msglenght;
    msgbuffer_t msgbuffer;
} client_message_t;

int client_message_create(client_message_t *self, FILE *input);

int client_message_destroy(client_message_t *self);

int client_message_realloc(client_message_t *self);

int client_message_send(client_message_t *self, info_client_t *info_client, 
                        uint32_t msg_id);

#endif