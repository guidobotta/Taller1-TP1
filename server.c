#define _POSIX_C_SOURCE 200112L
#include "socket.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    
    if (argc != 2) {
        printf("Error en la cantidad de parámetros.");
        return 1;
    }

    const char *servicename = argv[1];

    struct addrinfo hints;
    struct addrinfo *results, *addr_ptr;
    socket_t blsocket; //CAMBIAR NOMBRE
    socket_t peersocket;
    int status;
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // For bind

    status = getaddrinfo(0 /* ANY */, servicename, &hints, &results);

    if (!status) {
        printf("Error in getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    // Recorro resultados de getaddrinfo
    //ESTA BIEN ESTO??
    status = -1;
    addr_ptr = results;
    while ((addr_ptr != NULL) || (status != -1)) {
        if (socket_create(&blsocket) == -1) {
            printf("Error: %s\n", strerror(errno));
        } else {
            status = socket_bind(&blsocket, addr_ptr->ai_addr, addr_ptr->ai_addrlen);
            if (status == -1) {
                socket_destroy(&blsocket);
            }
        }
        addr_ptr = addr_ptr->ai_next;
    }

    if (socket_listen(&blsocket, 1) == -1) {
        printf("Error: %s\n", strerror(errno));
    } //CAMBIAR EL SOCKET ACCEPT!
    if (socket_accept(&blsocket, results->ai_addr, results->ai_addrlen, &peersocket) == -1) {
        printf("Error: %s\n", strerror(errno));
    }
    
    return 0;
}
