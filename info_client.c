#include "info_client.h"
#include "socket.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

int info_client_create(info_client_t *self, const char *hostname, const char *servicename){
    int status;

    self->hostname = hostname;
    self->servicename = servicename;

    memset(&(self->hints), 0, sizeof(struct addrinfo));
    (self->hints).ai_family = AF_INET;       // IPv4
    (self->hints).ai_socktype = SOCK_STREAM; // TCP
    (self->hints).ai_flags = 0;              // None

    status = getaddrinfo(self->hostname, self->servicename, &(self->hints), &(self->results));

    if (status != 0) {
        printf("Error in getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    return 0;
}

int info_client_destroy(info_client_t *self){
    freeaddrinfo(self->results);
    return 0;
}

int info_client_establish_connection(info_client_t *self){
    struct addrinfo *addr_ptr;

    // Recorro resultados de getaddrinfo
    for (addr_ptr = (self->results); addr_ptr != NULL; addr_ptr = addr_ptr->ai_next) {
        if (socket_create(&(self->clsocket)) == -1) {
            printf("Error: %s\n", strerror(errno));
        } else {
            if (socket_connect(&(self->clsocket), addr_ptr->ai_addr, addr_ptr->ai_addrlen) != -1) {
                break;
            }
            socket_destroy(&(self->clsocket));
        }
    }
    
    if (addr_ptr == NULL){
        printf("Error: Could not connect.");
        return 1;
    }
}