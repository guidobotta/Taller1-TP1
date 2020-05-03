#include "client_message.h"
#include "client_msgbuffer.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int client_msgbuffer_create(client_msgbuffer_t *self) {
    (self->buffer)[0] = '\0';
    return 0;
}

int client_msgbuffer_destroy(client_msgbuffer_t *self) {
    return 0;
}

int client_msgbuffer_getline(client_msgbuffer_t *self, client_message_t *msg, FILE *input) {
    if (fgets(self->buffer, 32, input) == NULL) return EOF;

    uint32_t n = (uint32_t)strlen(self->buffer);

    while (n == 31){
        (msg->msglenght) += n;
        if ((n + (msg->msglenght)) > (msg->msgmemory)) {
            client_message_realloc(msg);
        }
        strncat(msg->message, self->buffer, n);

        char* res = fgets(self->buffer, 32, input);

        n = (uint32_t)strlen(res);
    }

    (msg->msglenght) += n;
    if ((n + (msg->msglenght + 1)) > (msg->msgmemory)) {
        client_message_realloc(msg);
    }

    strncat(msg->message, self->buffer, n);
    return 0;
}