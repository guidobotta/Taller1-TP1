#ifndef __CLIENT_DBUS_PROTOCOL_H__
#define __CLIENT_DBUS_PROTOCOL_H__

#include <stdint.h>
#include "client_message.h"

typedef struct client_dbus_protocol {
    char* dbusheader;
    uint32_t header_size;
    uint32_t header_length;
    char* dbusbody;
    uint32_t body_size;
    uint32_t body_length;
} client_dbus_protocol_t;

/*
 * Inicializa el objeto protocolo.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_dbus_protocol_create(client_dbus_protocol_t *self);

/*
 * Destruye el objeto protocolo.
 *  Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_dbus_protocol_destroy(client_dbus_protocol_t *self);

/*
 * Convierte un mensaje tipo client_message_t en un mensaje de la
 * forma establecida en el protocolo DBUS.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_dbus_protocol_message_to_DBUS(client_dbus_protocol_t *self,
                            client_message_t *client_message, uint32_t msg_id);

#endif
