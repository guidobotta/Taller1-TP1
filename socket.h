#ifndef __TALLER_SOCKET_H__
#define __TALLER_SOCKET_H__

#include <sys/socket.h>
#include <unistd.h>

typedef struct socket {
    int socket;
} socket_t;

/*
 * Crea el socket.
 * Devuelve 0 si se creó correctamente o -1 si ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_create(socket_t* self);

/*
 * Destruye el socket.
 * Devuelve 0 si se destruyó correctamente o -1 si ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_destroy(socket_t* self);

/*
 * Enlaza un socket a una determinada dirección.
 * Devuelve 0 si se enlazó correctamente o -1 si ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_bind(socket_t* self, const struct sockaddr *address, 
                    socklen_t address_len);

/*
 * 
 * Devuelve 0 si se destruyó correctamente o -1 si ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_listen(socket_t* self, const struct sockaddr *address, 
                    socklen_t address_len);

/*
 *
*/
int socket_accept(socket_t* self);

/*
 * Intenta establecer una conexión en el socket.
 * Devuelve 0 si se conectó correctamente o -1 si ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_connect(socket_t* self, const struct sockaddr *address, 
                    socklen_t address_len);

/*
 *
*/
int socket_send(socket_t* self);

/*
 *
*/
int socket_recieve(socket_t* self);

/*
 *
*/
int socket_shutdown(socket_t* self);

#endif