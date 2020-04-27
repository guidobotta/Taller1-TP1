#ifndef __INFO_CLIENT_H__
#define __INFO_CLIENT_H__

#include "socket.h"
#include <netdb.h>

typedef struct info_client {
    const char *hostname;
    const char *servicename;
    struct addrinfo hints;
    struct addrinfo *results;
    socket_t clsocket;
} info_client_t;

int info_client_create(info_client_t *self, const char *hostname, const char *servicename);

int info_client_destroy(info_client_t *self);

int info_client_establish_connection(info_client_t *self);

#endif