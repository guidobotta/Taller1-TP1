#include "client_message.h"
#include "client_msgbuffer.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define ERROR 1
#define SUCCESS 0

//Muuy bueno que hayas abstraido el buffering en un TDA, perfecto.

int client_msgbuffer_create(client_msgbuffer_t *self) {
    (self->buffer)[0] = '\0';
    return SUCCESS;
}

int client_msgbuffer_destroy(client_msgbuffer_t *self) {
    return SUCCESS;
}

static int add_buff_to_msg(client_msgbuffer_t *self, client_message_t *msg, 
                    uint32_t n) {
    (msg->msglenght) += n;
    if ((msg->msglenght) + 1 > (msg->msgmemory)) {
        if (client_message_realloc(msg) == ERROR) {
            return ERROR;
        }
    }

    strncat(msg->message, self->buffer, n);

    return SUCCESS;
}

int client_msgbuffer_getline(client_msgbuffer_t *self, client_message_t *msg, 
                                FILE *input) {
    if (fgets(self->buffer, 32, input) == NULL) return EOF;

    uint32_t n = (uint32_t)strlen(self->buffer);

    while (n == 31){
        if (add_buff_to_msg(self, msg, n) == ERROR) {
            return ERROR;
        }

        if (fgets(self->buffer, 32, input) == NULL) {
            return SUCCESS;
        }

        n = (uint32_t)strlen(self->buffer);
    }

    if (add_buff_to_msg(self, msg, n) == ERROR) {
        return ERROR;
    }
    
    return SUCCESS;
}
