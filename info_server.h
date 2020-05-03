#ifndef __INFO_SERVER_H__
#define __INFO_SERVER_H__

#define _POSIX_C_SOURCE 201112L
#define __USE_XOPEN2K

#include "socket.h"
#include <netdb.h>

typedef struct info_server {
    const char *servicename;
    struct addrinfo hints;
    struct addrinfo *results;
    socket_t blsocket;
    socket_t peersocket;
} info_server_t;

int info_server_create(info_server_t *self, const char *servicename);

int info_server_destroy(info_server_t *self);

int info_server_establish_connection(info_server_t *self);

int info_server_send_client_confirmation(info_server_t *self);

#endif