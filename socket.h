#ifndef __TALLER_SOCKET_H__
#define __TALLER_SOCKET_H__

//#include <sys/types.h>
//#include <sys/socket.h>

typedef socket {
    int socket;
} socket_t;

/*
 * Crea el socket.
 * Devuelve 0 si se creó correctamente o -1 si ocurrió un error.
*/
int socket_create(socket_t* self);

/*
 * Destruye el socket.
 * Devuelve 0 si se destruyó correctamente o -1 si ocurrió un error.
*/
int socket_destroy(socket_t* self);

/*
 *
*/
int socket_bind(socket_t* self);

/*
 *
*/
int socket_listen(socket_t* self);

/*
 *
*/
int socket_accept(socket_t* self);

/*
 *
*/
int socket_connect(socket_t* self);

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