#include "client_message.h"
#include "client_msgbuffer.h"
#include "client_dbus_protocol.h"
#include "client_info.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ERROR 1
#define SUCCESS 0

int client_message_create(client_message_t *self, FILE *input) {
    int status;
    client_msgbuffer_create(&(self->client_msgbuffer));

    self->msgmemory = 64;
    self->message = calloc(self->msgmemory, sizeof(char));
    self->msglenght = 0;
    
    status = client_msgbuffer_getline(&(self->client_msgbuffer), self, input);
    
    if ((status == EOF) || (status == ERROR)) {
        return status;
    }

    return SUCCESS;
}

int client_message_send(client_message_t *self, client_info_t *client_info, 
                        uint32_t msg_id) {
    client_dbus_protocol_t client_dbus_protocol;
    client_dbus_protocol_create(&client_dbus_protocol);

    client_dbus_protocol_message_to_DBUS(&client_dbus_protocol, self, msg_id);
    client_info_send_message(client_info, &client_dbus_protocol);

    client_dbus_protocol_destroy(&client_dbus_protocol);

    return SUCCESS;
}

int client_message_destroy(client_message_t *self) {
    client_msgbuffer_destroy(&(self->client_msgbuffer));
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