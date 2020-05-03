#ifndef __SERVER_INFO_H__
#define __SERVER_INFO_H__

#define _POSIX_C_SOURCE 201112L
#define __USE_XOPEN2K

#include "common_socket.h"
#include <netdb.h>

typedef struct server_info {
    const char *servicename;
    struct addrinfo hints;
    struct addrinfo *results;
    socket_t blsocket;
    socket_t peersocket;
} server_info_t;

int server_info_create(server_info_t *self, const char *servicename);

int server_info_destroy(server_info_t *self);

int server_info_establish_connection(server_info_t *self);

int server_info_send_client_confirmation(server_info_t *self);

#endif