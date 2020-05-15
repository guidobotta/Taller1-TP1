#ifndef __CLIENT_CONTROLLER_H__
#define __CLIENT_CONTROLLER_H__

#include "client_info.h"
#include "client_message.h"

typedef struct client_controller{
    client_info_t client_info;
    FILE* input;
    client_message_t client_message;
    int status;
    uint32_t msg_id;
} client_controller_t;

/*
 * Inicializa el objeto client_controller.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_controller_create(client_controller_t *self, const char *hostname, 
                        const char *servicename, const char *path);

/*
 * Destruye el objeto client_controller.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_controller_destroy(client_controller_t *self);

/*
 * Pone en ejecución al cliente y su función de enviar
 * y recibir mensajes a un servidor.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_controller_execute(client_controller_t *self);

#endif
