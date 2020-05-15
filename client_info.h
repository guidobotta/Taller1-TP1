#ifndef __CLIENT_INFO_H__
#define __CLIENT_INFO_H__

#define _POSIX_C_SOURCE 201112L
#define __USE_XOPEN2K

typedef struct client_info client_info_t;

#include "common_socket.h"
#include "client_dbus_protocol.h"
#include <netdb.h>

struct client_info {
    const char *hostname;
    const char *servicename;
    struct addrinfo hints;
    struct addrinfo *results;
    socket_t clsocket;
};

/*
 * Inicializa el objeto client_info. Recibe la ip y el puerto
 * del servidor.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_info_create(client_info_t *self, const char *hostname, 
                        const char *servicename);

/*
 * Destruye el objeto client_info
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_info_destroy(client_info_t *self);

/*
 * Crea un socket y establece conexión con el servidor de la ip
 * y puerto recibida en la creación.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_info_establish_connection(client_info_t *self);

/*
 * Recibe un mensaje de tipoclient_message_t, lo pasa a formato 
 * client_dbus_protocol_t y lo envia al servidor conectado anteriormente.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_info_send_message(client_info_t *self, client_message_t *msg, 
                            uint32_t msg_id);

/*
 * Recibe un mensaje char* del servidor conectado anteriormente y
 * lo guarda en el buffer de tamaño n.
 * Devuelve 1 en caso de error o 0 en caso de éxito.
*/
int client_info_recibe_message(client_info_t *self, char* buff, size_t n);

#endif
