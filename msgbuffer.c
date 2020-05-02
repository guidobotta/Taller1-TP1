#include "client_message.h"
#include "msgbuffer.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int msgbuffer_create(msgbuffer_t *self) {
    (self->buffer)[0] = '\0';
    return 0;
}

int msgbuffer_destroy(msgbuffer_t *self) {
    return 0;
}

int msgbuffer_getline(msgbuffer_t *self, client_message_t *msg, FILE *input) {
    if (fgets(self->buffer, 32, input) == NULL) return EOF;

    uint32_t n = (uint32_t)strlen(self->buffer);

    while (n == 31){
        (msg->msglenght) += n;
        if ((n + (msg->msglenght)) > (msg->msgmemory)) {
            client_message_realloc(msg);
        }
        strncat(msg->message, self->buffer, n);
        fgets(self->buffer, 32, input);
        n = (uint32_t)strlen(self->buffer);
    }

    (msg->msglenght) += n;
    if ((n + (msg->msglenght + 1)) > (msg->msgmemory)) {
        client_message_realloc(msg);
    }

    strncat(msg->message, self->buffer, n);
    return 0;
}