#ifndef __SERVER_MESSAGE_H__
#define __SERVER_MESSAGE_H__

#include "server_info.h"
#include <stdint.h>

typedef struct server_message {
    char* message;
    uint32_t msg_length;
    uint32_t msg_id;
} server_message_t;

/*
 * Inicializa el objeto server_message.
 * Devuelve 1 en caso de error, 0 en caso de éxito o -1 en caso de
 * haber alcanzado el final del mensaje al cerrarse el socket cliente.
*/
int server_message_create(server_message_t *self, server_info_t *server_info);

/*
 * Destruye el objeto server_message.
 * Devuelve 1 en caso de error o 0 en caso de éxito
*/
int server_message_destroy(server_message_t *self);

/*
 * Imprime el mensaje recibido en la inicialización del objeto.
 * Devuelve 1 en caso de error o 0 en caso de éxito
*/
int server_message_print(server_message_t *self);

#endif
