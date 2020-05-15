#ifndef __SERVER_INFO_H__
#define __SERVER_INFO_H__

#define _POSIX_C_SOURCE 201112L
#define __USE_XOPEN2K

#include "common_socket.h"
#include <netdb.h>

typedef struct server_info {
    const char *servicename;
    struct addrinfo hints;
    struct addrinfo *results;
    socket_t blsocket;
    socket_t peersocket;
} server_info_t;

/*
 * Inicializa el objeto server_info. Recibe la ip a conectar el 
 * servidor.
 * Devuelve 1 en caso de error o 0 en caso de exito.
*/
int server_info_create(server_info_t *self, const char *servicename);

/*
 * Destruye el objeto server_info.
 * Devuelve 1 en caso de error o 0 en caso de exito.
*/
int server_info_destroy(server_info_t *self);

/*
 * Crea un socket, establece conexión con el cliente y establece
 * la conexión de un socket peer con el socket cliente.
 * Devuelve 1 en caso de error o 0 en caso de exito.
*/
int server_info_establish_connection(server_info_t *self);

/*
 * Recibe un mensaje de tamaño n del cliente y lo guarda
 * en el buffer.
 * Devuelve -1 en caso de error o la cantidad de bytes leidos
 * en caso de exito.
*/
int server_info_rcv_message(server_info_t *self, char *buff, size_t n);

/*
 * Le envia un mensaje msg de tamaño n al cliente.
 * Devuelve -1 en caso de error o la cantidad de byter enviados
 * en caso de exito.
*/
int server_info_send_message(server_info_t *self, char *msg, size_t n);

#endif
