#ifndef __CLIENT_INFO_H__
#define __CLIENT_INFO_H__

#define _POSIX_C_SOURCE 201112L
#define __USE_XOPEN2K

typedef struct client_info client_info_t;

#include "common_socket.h"
#include "client_dbus_protocol.h"
#include <netdb.h>

struct client_info {
    const char *hostname;
    const char *servicename;
    struct addrinfo hints;
    struct addrinfo *results;
    socket_t clsocket;
};

int client_info_create(client_info_t *self, const char *hostname, 
                        const char *servicename);

int client_info_destroy(client_info_t *self);

int client_info_establish_connection(client_info_t *self);

int client_info_send_message(client_info_t *self, 
                                client_dbus_protocol_t *client_dbus_protocol);

int client_info_recibe_confirmation(client_info_t *self);

#endif