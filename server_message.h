#ifndef __SERVER_MESSAGE_H__
#define __SERVER_MESSAGE_H__

#include "server_info.h"
#include <stdint.h>

typedef struct server_message {
    char* message;
    uint32_t msg_length;
    uint32_t msg_id;
} server_message_t;

int server_message_create(server_message_t *self, server_info_t *server_info);

int server_message_destroy(server_message_t *self);

int server_message_print(server_message_t *self);

#endif