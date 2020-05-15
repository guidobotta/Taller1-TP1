#include "server_info.h"
#include "common_socket.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define SOCKET_ERROR -1
#define ERROR 1
#define SUCCESS 0

int server_info_create(server_info_t *self, const char *servicename){
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
        return ERROR;
    }

    return SUCCESS;
}

int server_info_destroy(server_info_t *self){
    freeaddrinfo(self->results);
    
    if (socket_shutdown((&self->blsocket), SHUT_RDWR) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    if (socket_shutdown((&self->peersocket), SHUT_RDWR) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    if (socket_destroy((&self->blsocket)) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    if (socket_destroy((&self->peersocket)) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }
    
    return SUCCESS;
}

int server_info_establish_connection(server_info_t *self){
    struct addrinfo *addr_ptr;

    // Recorro resultados de getaddrinfo
    for (addr_ptr = (self->results); addr_ptr != NULL; 
            addr_ptr = addr_ptr->ai_next) {
        if (socket_create(&(self->blsocket)) == SOCKET_ERROR) {
            printf("Error: %s\n", strerror(errno));
        } else {
            if ( (socket_bind(&(self->blsocket), addr_ptr->ai_addr, 
                    addr_ptr->ai_addrlen) != SOCKET_ERROR) &&
                    (socket_listen(&(self->blsocket), 10) != SOCKET_ERROR) ) {
                break;
            } else {
                printf("Error: %s\n", strerror(errno));
            }
            socket_destroy(&(self->blsocket));
        }
    }
    
    if (addr_ptr == NULL){
        printf("Error: Could not bind.");
        return ERROR;
    }

    if (socket_accept(&(self->blsocket), addr_ptr->ai_addr, 
            &(addr_ptr->ai_addrlen), &(self->peersocket)) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    return SUCCESS;
}

int server_info_rcv_message(server_info_t *self, char *buff, size_t n) {
    return socket_receive(&(self->peersocket), buff, n, 0);
}

int server_info_send_message(server_info_t *self, char *msg, size_t n) {
    return socket_send(&(self->peersocket), msg, n, 0) == SOCKET_ERROR;
}
