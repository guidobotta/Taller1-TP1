#include "client_message.h"
#include "msgbuffer.h"
#include "dbus_protocol.h"
#include "info_client.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

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

int client_message_send(client_message_t *self, info_client_t *info_client, 
                        uint32_t msg_id) {
    dbus_protocol_t dbus_protocol;
    dbus_protocol_create(&dbus_protocol);

    dbus_protocol_message_to_DBUS(&dbus_protocol, self, msg_id);
    info_client_send_message(info_client, &dbus_protocol);

    dbus_protocol_destroy(&dbus_protocol);

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