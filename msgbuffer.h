#ifndef __MSGBUFFER_H__
#define __MSGBUFFER_H__

#include "client_message.h"
#include <stdio.h>

typedef struct msgbuffer {
    char buffer[32];
} msgbuffer_t;

int msgbuffer_create(msgbuffer_t *self);

int msgbuffer_destroy(msgbuffer_t *self);

int msgbuffer_getline(msgbuffer_t *self, client_message_t *msg, FILE *input);

#endif