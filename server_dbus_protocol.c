#include <stdlib.h>
#include <stdio.h>
#include "server_dbus_protocol.h"
#include "server_message.h"
#include "common_socket.h"

#define ERROR 1
#define SUCCESS 0

int server_dbus_protocol_create(server_dbus_protocol_t *self, 
                            server_info_t *server_info) {
    (self->dbusheader) = calloc(16, sizeof(char));

    int status = socket_receive(&(server_info->peersocket), 
                                self->dbusheader, 16, 0);

    if (status == 0) {
        free(self->dbusheader);
        return EOF;
    } else if (status == -1) {
        free(self->dbusheader);
        return ERROR;
    }

    uint32_t body_length;
    uint32_t array_length;
    uint32_t msg_id;
    
    char* char_to_int = (char*)&body_length;
    char_to_int[0] = self->dbusheader[4];
    char_to_int[1] = self->dbusheader[5];
    char_to_int[2] = self->dbusheader[6];
    char_to_int[3] = self->dbusheader[7];

    if (body_length == 4) {
        body_length = 0;
    }

    char_to_int = (char*)&msg_id;
    char_to_int[0] = self->dbusheader[8];
    char_to_int[1] = self->dbusheader[9];
    char_to_int[2] = self->dbusheader[10];
    char_to_int[3] = self->dbusheader[11];

    char_to_int = (char*)&array_length;
    char_to_int[0] = self->dbusheader[12];
    char_to_int[1] = self->dbusheader[13];
    char_to_int[2] = self->dbusheader[14];
    char_to_int[3] = self->dbusheader[15];

    (self->id) = msg_id;
    (self->header_length) = array_length;
    (self->body_length) = body_length;

    self->dbusheader = realloc(self->dbusheader, self->header_length);
    socket_receive(&(server_info->peersocket), self->dbusheader,
                    self->header_length, 0);

    self->dbusbody = calloc(self->body_length, sizeof(char));
    socket_receive(&(server_info->peersocket), self->dbusbody, 
                    self->body_length, 0);

    return SUCCESS;
}

int server_dbus_protocol_destroy(server_dbus_protocol_t *self) {
    free(self->dbusbody);
    free(self->dbusheader);
    return SUCCESS;
}

int server_dbus_protocol_DBUS_to_message(server_dbus_protocol_t *self, 
                                        server_message_t *server_message) {
    uint32_t dbus_header_index = 0;
    (server_message->msg_length) = 0;

    (server_message->message) = calloc((self->header_length) + 
                                            (self->body_length), sizeof(char));

    uint32_t param_size = 0;
    uint32_t wordnumber = 0;
    char* char_to_int;

    server_message->msg_id = self->id;

    while (wordnumber < 4) {
        dbus_header_index += 4;

        char_to_int = (char*)&param_size;
        char_to_int[0] = (self->dbusheader)[dbus_header_index];
        dbus_header_index++;
        char_to_int[1] = (self->dbusheader)[dbus_header_index];
        dbus_header_index++;
        char_to_int[2] = (self->dbusheader)[dbus_header_index];
        dbus_header_index++;
        char_to_int[3] = (self->dbusheader)[dbus_header_index];
        dbus_header_index++;

        param_size++;

        for (uint32_t i = 0; i < param_size; i++) {
            (server_message->message)[server_message->msg_length] = 
                                    (self->dbusheader)[dbus_header_index];
            dbus_header_index++;
            (server_message->msg_length)++;
        }

        while ((param_size % 8) != 0) {
            param_size ++;
            dbus_header_index++;
        }

        wordnumber++;
    }

    uint32_t dbus_body_index = 0;

    while (dbus_body_index <  (self->body_length)) {
        char_to_int = (char*)&param_size;
        char_to_int[0] = (self->dbusbody)[dbus_body_index];
        dbus_body_index++;
        char_to_int[1] = (self->dbusbody)[dbus_body_index];
        dbus_body_index++;
        char_to_int[2] = (self->dbusbody)[dbus_body_index];
        dbus_body_index++;
        char_to_int[3] = (self->dbusbody)[dbus_body_index];
        dbus_body_index++;

        param_size++;

        for (uint32_t i = 0; i < param_size; i++) {
            (server_message->message)[server_message->msg_length] = 
                                        (self->dbusbody)[dbus_body_index];
            dbus_body_index++;
            (server_message->msg_length)++;
        }
    }

    return 0;
}
