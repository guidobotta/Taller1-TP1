#ifndef __SERVER_MESSAGE_H__
#define __SERVER_MESSAGE_H__

#include "info_server.h"
#include <stdint.h>

typedef struct server_message {
    char* message;
    uint32_t msg_length;
} server_message_t;

int server_message_create(server_message_t *self, info_server_t *info_server);

int server_message_destroy(server_message_t *self);

int server_message_print(server_message_t *self);

#endif