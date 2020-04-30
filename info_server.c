#include "info_server.h"
#include "socket.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

int info_server_create(info_server_t *self, const char *servicename){
    int status;

    self->servicename = servicename;

    memset(&(self->hints), 0, sizeof(struct addrinfo));
    (self->hints).ai_family = AF_INET;       // IPv4
    (self->hints).ai_socktype = SOCK_STREAM; // TCP
    (self->hints).ai_flags = AI_PASSIVE;     // For bind

    status = getaddrinfo(0, self->servicename, &(self->hints), 
                            &(self->results));

    if (status != 0) {
        printf("Error in getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    return 0;
}

int info_server_destroy(info_server_t *self){
    freeaddrinfo(self->results);
    return 0;
}

int info_server_establish_connection(info_server_t *self){
    struct addrinfo *addr_ptr;

    // Recorro resultados de getaddrinfo
    for (addr_ptr = (self->results); addr_ptr != NULL; 
            addr_ptr = addr_ptr->ai_next) {
        if (socket_create(&(self->blsocket)) == -1) {
            printf("Error: %s\n", strerror(errno));
        } else {
            if (socket_bind(&(self->blsocket), addr_ptr->ai_addr, 
                                addr_ptr->ai_addrlen) != -1) {
                break;
            }
            socket_destroy(&(self->blsocket));
        }
    }
    
    if (addr_ptr == NULL){
        printf("Error: Could not bind.");
        return 1;
    }

    if (socket_listen(&(self->blsocket), 10) == -1) {
        printf("Error: %s\n", strerror(errno));
    }
    if (socket_accept(&(self->blsocket), addr_ptr->ai_addr, 
                        &(addr_ptr->ai_addrlen), &(self->peersocket)) == -1) {
        printf("Error: %s\n", strerror(errno));
    }
}
