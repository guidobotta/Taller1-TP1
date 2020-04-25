#define _POSIX_C_SOURCE 201112L
#include "socket.h"
#include <sys/socket.h>
#include <unistd.h>

int socket_create(socket_t* self) {
    self->socket = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP, ANY
}

int socket_destroy(socket_t* self) {
    return close(self->socket);
}

int socket_bind(socket_t* self, const struct sockaddr *address, 
                    socklen_t address_len) {
    return bind(self->socket, address, address_len);
}

int socket_listen(socket_t* self, int backlog) {
    return listen(self->socket, backlog);
}

int socket_accept(socket_t* self, struct sockaddr *restrict address,
                    socklen_t *restrict address_len, socket_t* new) {
    return (new->socket = accept(self->socket, address, address_len));
}

int socket_connect(socket_t* self, const struct sockaddr *address, 
                    socklen_t address_len) {
    return connect(self->socket, address, address_len);
}

int socket_send(socket_t* self) {
    // Do Something
}

int socket_recieve(socket_t* self) {
    // Do Something
}

int socket_shutdown(socket_t* self) {
    // Do Something
}