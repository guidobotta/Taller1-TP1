#ifndef __SERVER_DBUS_PROTOCOL_H__
#define __SERVER_DBUS_PROTOCOL_H__

#include <stdint.h>
#include "server_dbus_protocol.h"
#include "server_message.h"

typedef struct server_dbus_protocol {
    char* dbusheader;
    uint32_t header_length;
    char* dbusbody;
    uint32_t body_length;
    uint32_t id;
} server_dbus_protocol_t;

/*
 * Inicializa el objeto server_dbus_protocol.
 * Devuelve 1 en caso de error, 0 en caso de éxito o -1 en
 * caso de que se haya cerrado el socket cliente.
*/
int server_dbus_protocol_create(server_dbus_protocol_t *self, 
                            server_info_t *server_info);

/*
 * Destruye el objeto server_dbus_protocol.
 * Devuelve 1 en caso de error o 0 en caso de éxito
*/
int server_dbus_protocol_destroy(server_dbus_protocol_t *self);

/*
 * Recibe un objeto server_message y lo setea con el
 * mensaje que recibió al inicializarse.
 * Devuelve 1 en caso de error o 0 en caso de éxito
*/
int server_dbus_protocol_DBUS_to_message(server_dbus_protocol_t *self, 
                                        server_message_t *server_message);

#endif
