#include "client_info.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define SOCKET_ERROR -1
#define ERROR 1
#define SUCCESS 0

int client_info_create(client_info_t *self, const char *hostname, 
                        const char *servicename){
    int status;

    self->hostname = hostname;
    self->servicename = servicename;

    memset(&(self->hints), 0, sizeof(struct addrinfo));
    (self->hints).ai_family = AF_INET;       // IPv4
    (self->hints).ai_socktype = SOCK_STREAM; // TCP
    (self->hints).ai_flags = 0;              // None

    status = getaddrinfo(self->hostname, self->servicename, 
                            &(self->hints), &(self->results));

    if (status != 0) {
        printf("Error in getaddrinfo: %s\n", gai_strerror(status));
        return ERROR;
    }

    return SUCCESS;
}

int client_info_destroy(client_info_t *self){
    freeaddrinfo(self->results);

    if (socket_shutdown((&self->clsocket), SHUT_RDWR) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    if (socket_destroy((&self->clsocket)) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    return SUCCESS;
}

int client_info_establish_connection(client_info_t *self){
    struct addrinfo *addr_ptr;

    // Recorro resultados de getaddrinfo
    for (addr_ptr = (self->results); addr_ptr != NULL; 
            addr_ptr = addr_ptr->ai_next) {
        if (socket_create(&(self->clsocket)) == SOCKET_ERROR) {
            printf("Error: %s\n", strerror(errno));
        } else {
            if (socket_connect(&(self->clsocket), addr_ptr->ai_addr, 
                addr_ptr->ai_addrlen) != SOCKET_ERROR) {
                break;
            } else {
                printf("Error: %s\n", strerror(errno));
            }
            if (socket_destroy(&(self->clsocket)) == SOCKET_ERROR) {
                printf("Error: %s\n", strerror(errno));
            }
        }
    }
    
    if (addr_ptr == NULL){
        printf("Error: Could not connect.");
        return ERROR;
    }

    return SUCCESS;
}

int client_info_send_message(client_info_t *self, 
                                client_dbus_protocol_t *client_dbus_protocol) {
    if (socket_send(&(self->clsocket), client_dbus_protocol->dbusheader, 
                client_dbus_protocol->header_length, 0) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    if (socket_send(&(self->clsocket), client_dbus_protocol->dbusbody, 
                client_dbus_protocol->body_length, 0) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    return SUCCESS;
}

int client_info_recibe_confirmation(client_info_t *self, uint32_t id) {
    char confirmation[3];

    if (socket_receive(&(self->clsocket), confirmation, 3, 0) == 
                        SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    printf("0x%08x: %s\n", id, confirmation);

    return SUCCESS;
}
