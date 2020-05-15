#ifndef __CLIENT_MMESSAGE_H__
#define __CLIENT_MMESSAGE_H__

typedef struct client_message client_message_t;

#include "client_msgbuffer.h"
#include "client_info.h"
#include <stdint.h>
#include <stdio.h>

struct client_message {
    FILE *input;
    char* message;
    uint32_t msgmemory;
    uint32_t msglenght;
    client_msgbuffer_t client_msgbuffer;
};

/*
 * Inicializa el objeto client_message. Recibe el file
 * de donde se leera el mensaje.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_message_create(client_message_t *self, const char *path);

/*
 * Destruye el objeto client_message.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_message_destroy(client_message_t *self);

/*
 * Relocaliza el mensaje en un mayor espacio de memoria.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_message_realloc(client_message_t *self);

#endif
