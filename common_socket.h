#ifndef __TALLER_SOCKET_H__
#define __TALLER_SOCKET_H__

#include <unistd.h>
#include <sys/socket.h>

typedef struct socket {
    int socket;
} socket_t;

/*
 * Crea el socket.
 * Devuelve 0 si se creó correctamente o -1 si ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_create(socket_t *self);

/*
 * Destruye el socket.
 * Devuelve 0 si se destruyó correctamente o -1 si ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_destroy(socket_t *self);

/*
 * Enlaza un socket a una determinada dirección.
 * Devuelve 0 si se enlazó correctamente o -1 si ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_bind(socket_t *self, const struct sockaddr *address, 
                    socklen_t address_len);

/*
 * Escucha conexiones entrantes.
 * El argumento backlog indica el limite de conexiones en la cola de
 * sockets entrantes.
 * Devuelve 0 si se completó correctamente o -1 si ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_listen(socket_t *self, int backlog);

/* 
 * Extrae la primera conexión en la cola de sockets entrantes, crea un
 * socket nuevo y aloca un nuevo file descriptor para ese socket.
 * Devuelve un file descriptor no negativo del socket aceptado o -1 si
 * ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_accept(socket_t *self, struct sockaddr *restrict address,
                    socklen_t *restrict address_len, socket_t *new);

/*
 * Intenta establecer una conexión en el socket.
 * Devuelve 0 si se conectó correctamente o -1 si ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_connect(socket_t *self, const struct sockaddr *address, 
                    socklen_t address_len);

/*
 * Transmite un mensaje de un socket a su peer. El buffer debe contener
 * el mensaje a ser enviado. Se debe especificar la longitud del
 * buffer en bytes.
 * Devuelve el numero de bytes enviados o -1 si ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_send(socket_t *self, const char *buffer, size_t length, int flags);

/*
 * Recibe un mensaje de un sockety lo guarda en el buffer. Se debe
 * especificar la longitud en bytes del buffer.
 * Devuelve la longitud del mensaje en bytes o 0 si no había mensajes
 * disponibles y el peer se cerró o -1 si ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_receive(socket_t *self, char *buffer, size_t length, int flags);

/*
 * Cierra parte o toda la conexión de la comunicación un socket.
 * Devuelve 0 si se cerró correctamente o -1 si ocurrió un error.
 * Si ocurre un error setea errno.
*/
int socket_shutdown(socket_t *self, int how);

#endif
