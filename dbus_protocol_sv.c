#include <stdlib.h>
#include "dbus_protocol_sv.h"
#include "server_message.h"
#include "socket.h"

int dbus_protocol_sv_create(dbus_protocol_sv_t *self, info_server_t *info_server {
    (self->dbusheader) = calloc(self->header_length, 16);

    socket_receive(&(info_server->peersocket), self->dbusheader, 16, 0);

    uint32_t body_length;
    uint32_t array_length;
    
    char* char_to_int = (char*)&body_length;
    char_to_int[0] = (self->dbusheader)[4];
    char_to_int[1] = (self->dbusheader)[5];
    char_to_int[2] = (self->dbusheader)[6];
    char_to_int[3] = (self->dbusheader)[7];

    if (body_length == 4) {
        body_length = 0;
    }

    char_to_int = (char*)&array_length;
    char_to_int[0] = (self->dbusheader)[12];
    char_to_int[1] = (self->dbusheader)[13];
    char_to_int[2] = (self->dbusheader)[14];
    char_to_int[3] = (self->dbusheader)[15];

    (self->body_length) = body_length;
    self->dbusbody = calloc(self->body_length, sizeof(char));

    (self->header_length) = array_length + 16;
    self->dbusheader = realloc(self->header_length, sizeof(char));

    socket_receive(&(info_server->peersocket), &((self->dbusheader)[16]),
                    (self->header_length) - 16, 0);

    socket_receive(&(info_server->peersocket), self->dbusbody, 
                    self->body_length, 0);

    return 0;
}

int dbus_protocol_sv_destroy(dbus_protocol_sv_t *self) {
    free(self->dbusbody);
    free(self->dbusheader);
    return 0;
}

int dbus_protocol_sv_DBUS_to_message(dbus_protocol_sv_t *self, server_message_t *server_message) {
    uint32_t dbus_index = 0;
    uint32_t msg_index = 0;

    (server_message->message) = calloc((self->header_length) + 
                                            (self->body_length), sizeof(char));

    dbus_index += 16;

    uint32_t param_size = 0;
    char* char_to_int;

    while (dbus_index < (self->header_length)) {
        dbus_index += 4;

        char_to_int = (char*)&param_size;
        char_to_int[0] = (self->dbusheader)[dbus_index];
        dbus_index++;
        char_to_int[1] = (self->dbusheader)[dbus_index];
        dbus_index++;
        char_to_int[2] = (self->dbusheader)[dbus_index];
        dbus_index++;
        char_to_int[3] = (self->dbusheader)[dbus_index];
        dbus_index++;

        param_size++;

        for (uint32_t i; i < param_size; i++) {
            (server_message->message)[msg_index] = (self->dbusheader)[dbus_index];
            dbus_index++;
            msg_index++;
        }

        while ((param_size % 8) != 0) {
            param_size ++;
            dbus_index++;
        }
    }

    while (dbus_index < ((self->header_length) + (self->body_length))) {
        char_to_int = (char*)&param_size;
        char_to_int[0] = (self->dbusheader)[dbus_index];
        dbus_index++;
        char_to_int[1] = (self->dbusheader)[dbus_index];
        dbus_index++;
        char_to_int[2] = (self->dbusheader)[dbus_index];
        dbus_index++;
        char_to_int[3] = (self->dbusheader)[dbus_index];
        dbus_index++;

        param_size++;

        for (uint32_t i; i < param_size; i++) {
            (server_message->message)[msg_index] = (self->dbusheader)[dbus_index];
            dbus_index++;
            msg_index++;
        }
    }

    return 0;
}