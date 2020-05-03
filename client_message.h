#ifndef __CLIENT_MMESSAGE_H__
#define __CLIENT_MMESSAGE_H__

typedef struct client_message client_message_t;

#include "client_msgbuffer.h"
#include "client_info.h"
#include <stdint.h>

struct client_message {
    char* message;
    uint32_t msgmemory;
    uint32_t msglenght;
    client_msgbuffer_t client_msgbuffer;
};

int client_message_create(client_message_t *self, FILE *input);

int client_message_destroy(client_message_t *self);

int client_message_realloc(client_message_t *self);

int client_message_send(client_message_t *self, client_info_t *client_info, 
                        uint32_t msg_id);

#endif