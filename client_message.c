#include "client_message.h"
#include "msgbuffer.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
int _check_msg(client_message_t *self){

}
*/

int client_message_create(client_message_t *self, FILE *input){
    msgbuffer_create(&(self->msgbuffer));

    self->msgmemory = 64;
    self->message = calloc(self->msgmemory, sizeof(char));
    
    return msgbuffer_getline(&(self->msgbuffer), self, input);
}

int client_message_destroy(client_message_t *self){
    msgbuffer_destroy(&(self->msgbuffer));
    free(self->message);
}

int client_message_realloc(client_message_t *self){
    self->message = realloc(self->message, ((self->msgmemory) += 64));
}

int client_message_to_DBUS(client_message_t *self){
    return 0;
}
