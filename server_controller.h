#ifndef __SERVER_CONTROLLER_H__
#define __SERVER_CONTROLLER_H__

#include "server_info.h"
#include "server_message.h"

typedef struct server_controller {
    server_info_t server_info;
    server_message_t server_message;
    int status;
} server_controller_t;

/*
 * Inicializa el objeto server_controller.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int server_controller_create(server_controller_t *self, 
                            const char *servicename);

/*
 * Destruye el objeto server_controller.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int server_controller_destroy(server_controller_t *self);

/*
 * Pone en ejecución al servidor y su función de
 * recibir y enviar mensajes a un cliente.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int server_controller_execute(server_controller_t *self);

#endif
