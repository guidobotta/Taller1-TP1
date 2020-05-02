#include "socket.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int socket_create(socket_t *self) {
    self->socket = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP, ANY
    return 0;
}

int socket_destroy(socket_t *self) {
    return close(self->socket);
}

int socket_bind(socket_t *self, const struct sockaddr *address, 
                    socklen_t address_len) {
    return bind(self->socket, address, address_len);
}

int socket_listen(socket_t *self, int backlog) {
    return listen(self->socket, backlog);
}

int socket_accept(socket_t *self, struct sockaddr *restrict address,
                    socklen_t *restrict address_len, socket_t *new) {
    return (new->socket = accept(self->socket, address, address_len));
}

int socket_connect(socket_t *self, const struct sockaddr *address, 
                    socklen_t address_len) {
    return connect(self->socket, address, address_len);
}

int socket_send(socket_t *self, const char *buffer, size_t length, int flags){
    size_t bytes_sent = 0;
    size_t status;
    
    while ((bytes_sent < length) && (status != 0)){
        status = send(self->socket, &(buffer[bytes_sent]), length, flags);
        
        if (status == -1){
            printf("Error: %s\n", strerror(errno));
            return -1;
        }

        bytes_sent += status;
    }
    
    
    return (int)bytes_sent;
}

int socket_receive(socket_t *self, char *buffer, size_t length, int flags){
    size_t bytes_recv = 0;
    size_t status;
    
    while ((bytes_recv < length) && (status != 0)){
        status = recv(self->socket, &(buffer[bytes_recv]), length, flags);
        
        if (status == -1){
            printf("Error: %s\n", strerror(errno));
            return -1;
        }

        bytes_recv += status;
    }
    
    return (int)bytes_recv;
}

int socket_shutdown(socket_t *self, int how){
    return shutdown(self->socket, how);
}
