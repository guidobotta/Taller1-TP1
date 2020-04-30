#ifndef __INFO_CLIENT_H__
#define __INFO_CLIENT_H__

#define _POSIX_C_SOURCE 201112L
#define __USE_XOPEN2K

typedef struct info_client info_client_t;

#include "socket.h"
#include "dbus_protocol.h"
#include <netdb.h>

typedef struct info_client {
    const char *hostname;
    const char *servicename;
    struct addrinfo hints;
    struct addrinfo *results;
    socket_t clsocket;
} info_client_t;

int info_client_create(info_client_t *self, const char *hostname, 
                        const char *servicename);

int info_client_destroy(info_client_t *self);

int info_client_establish_connection(info_client_t *self);

int info_client_send_message(info_client_t *self, 
                                dbus_protocol_t *dbus_protocol);

#endif