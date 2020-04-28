#include "client_message.h"
#include "msgbuffer.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR 1
#define SUCCESS 0

int client_message_create(client_message_t *self, FILE *input) {
    int status;
    msgbuffer_create(&(self->msgbuffer));

    self->msgmemory = 64;
    self->message = calloc(self->msgmemory, sizeof(char));
    self->msglenght = 0;
    
    status = msgbuffer_getline(&(self->msgbuffer), self, input);
    
    if ((status == EOF) || (status == ERROR)) {
        return status;
    }

    return SUCCESS;
}

int client_message_destroy(client_message_t *self) {
    msgbuffer_destroy(&(self->msgbuffer));
    free(self->message);
    return SUCCESS;
}

int client_message_realloc(client_message_t *self) {
    self->message = realloc(self->message, ((self->msgmemory) += 64));
    if (self->message == NULL){
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }
    return SUCCESS;
}

int client_message_to_DBUS(client_message_t *self) {
    return 0;
}