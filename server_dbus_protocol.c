#include <stdlib.h>
#include <stdio.h>
#include "server_dbus_protocol.h"
#include "server_message.h"
#include "common_socket.h"

#define SOCKET_ERROR -1
#define ERROR 1
#define SUCCESS 0

static void char_to_int32(uint32_t *param, char* buff, uint32_t *index) {
    char *char_to_int = (char*)param;
    char_to_int[0] = (buff)[*index];
    (*index)++;
    char_to_int[1] = (buff)[*index];
    (*index)++;
    char_to_int[2] = (buff)[*index];
    (*index)++;
    char_to_int[3] = (buff)[*index];
    (*index)++;
}

static int get_protocol_values(server_dbus_protocol_t *self, 
                                server_info_t *server_info) {
    (self->dbusheader) = calloc(16, sizeof(char));

    int status = server_info_rcv_message(server_info, self->dbusheader, 16);

    if (status == 0) {
        free(self->dbusheader);
        return EOF;
    } else if (status == SOCKET_ERROR) {
        free(self->dbusheader);
        return ERROR;
    }

    return SUCCESS;
}

static void set_protocol_values(server_dbus_protocol_t *self) {
    uint32_t index = 4;

    char_to_int32(&(self->body_length), self->dbusheader, &index);
    if ((self->body_length) == 4) {
        (self->body_length) = 0;
    }
    char_to_int32(&(self->id), self->dbusheader, &index);
    char_to_int32(&(self->header_length), self->dbusheader, &index);
}

static int get_header(server_dbus_protocol_t *self, server_info_t 
                        *server_info) {
    char* result = realloc(self->dbusheader, self->header_length);

    if (result == NULL) {
        free(self->dbusheader);
        return ERROR;
    } else {
        self->dbusheader = result;
    }
    
    if (server_info_rcv_message(server_info, self->dbusheader, 
                                self->header_length) == SOCKET_ERROR) {
        return ERROR;
    }

    return SUCCESS;
}

static int get_body(server_dbus_protocol_t *self, server_info_t *server_info) {
    self->dbusbody = calloc(self->body_length, sizeof(char));

    if (self->dbusbody == NULL) {
        return ERROR;
    }
    
    if (server_info_rcv_message(server_info, self->dbusbody, 
                                self->body_length) == SOCKET_ERROR) {
        return ERROR;
    }

    return SUCCESS;
}

int server_dbus_protocol_create(server_dbus_protocol_t *self, 
                                server_info_t *server_info) {
    int status;
    if ((status = get_protocol_values(self, server_info)) != SUCCESS) {
        return status;
    }
    set_protocol_values(self);

    if (get_header(self, server_info) == ERROR) {
        return ERROR;
    }

    if (get_body(self, server_info) == ERROR) {
        return ERROR;
    }

    return SUCCESS;
}

int server_dbus_protocol_destroy(server_dbus_protocol_t *self) {
    free(self->dbusbody);
    free(self->dbusheader);
    return SUCCESS;
}

static void set_param(char* buff, server_message_t *server_message, 
                        uint32_t *index, uint32_t *param_size) {
    char_to_int32(param_size, buff, index);
    (*param_size)++;

    for (uint32_t i = 0; i < (*param_size); i++) {
        (server_message->message)[server_message->msg_length] = 
                                                (buff)[*index];
        (*index)++;
        (server_message->msg_length)++;
    }
}

static void look_for_next_param(uint32_t *param_size, uint32_t *index) {
    while ((*param_size % 8) != 0) {
        (*param_size)++;
        (*index)++;
    }
}

int server_dbus_protocol_DBUS_to_message(server_dbus_protocol_t *self, 
                                        server_message_t *server_message) {
    server_message->msg_id = self->id;
    (server_message->msg_length) = 0;
    uint32_t dbus_header_index = 0;
    uint32_t dbus_body_index = 0;
    uint32_t param_size = 0;

    (server_message->message) = calloc((self->header_length) + 
                                            (self->body_length), sizeof(char));
    if ((server_message->message) == NULL) {
        return ERROR;
    }

    for (int i = 0; i < 4; i++) {
        dbus_header_index += 4;
        set_param(self->dbusheader, server_message, &dbus_header_index, 
                    &param_size);   
        look_for_next_param(&param_size, &dbus_header_index);
    }

    while (dbus_body_index < (self->body_length)) {
        set_param(self->dbusbody, server_message, &dbus_body_index, 
                    &param_size);
    }

    return SUCCESS;
}
