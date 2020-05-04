#ifndef __CLIENT_MSGBUFFER_H__
#define __CLIENT_MSGBUFFER_H__

#include "client_message.h"
#include <stdio.h>

typedef struct client_msgbuffer {
    char buffer[32];
} client_msgbuffer_t;

/*
 * Inicializa buffer.
 * Devuelve 1 en caso de error o 0 en caso de exito.
*/
int client_msgbuffer_create(client_msgbuffer_t *self);

/*
 * Destruye buffer.
 * Devuelve 1 en caso de error o 0 en caso de exito.
*/
int client_msgbuffer_destroy(client_msgbuffer_t *self);

/*
 * Lee una linea hasta un '\n' del archivo input. Guarda la
 * linea en el objeto client_message_t.
 * Devuelve 1 en caso de error o 0 en caso de exito.
*/
int client_msgbuffer_getline(client_msgbuffer_t *self, client_message_t *msg, 
                                FILE *input);

#endif
