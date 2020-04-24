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
        return 1;
    }

    const char *servicename = argv[1];

    struct addrinfo hints;
    struct addrinfo *results, *addr_ptr;
    socket_t blsocket; //CAMBIAR NOMBRE
    int status;
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // For bind

    status = getaddrinfo(0 /* ANY */, servicename, &hints, &results);

    if (!status){
        printf("Error in getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    blsocket = socket_create(&blsocket);
    socket_bind();
    socket_listen();
    
    return 0;
}
