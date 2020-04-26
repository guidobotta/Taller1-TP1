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
    
    if ((argc < 3) || (argc > 4)) {
        printf("Error en la cantidad de parámetros.");
        return 1;
    }

    const char *hostname = argv[1];
    const char *servicename = argv[2];
    const FILE *input;

    if (argc == 3) {
        input = stdin;
    } else {
        input = fopen(argv[3], "r");
    }

    struct addrinfo hints;
    struct addrinfo *results, *addr_ptr;
    socket_t clsocket;
    int status;
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = 0;              // None

    status = getaddrinfo(hostname, servicename, &hints, &results);

    if (status != 0) {
        printf("Error in getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    // Recorro resultados de getaddrinfo
    for (addr_ptr = results; addr_ptr != NULL; addr_ptr = addr_ptr->ai_next) {
        if (socket_create(&clsocket) == -1) {
            printf("Error: %s\n", strerror(errno));
        } else {
            if (socket_connect(&clsocket, addr_ptr->ai_addr, addr_ptr->ai_addrlen) != -1) {
                break;
            }
            socket_destroy(&clsocket);
        }
    }
    
    if (addr_ptr == NULL){
        printf("Error: Could not connect.");
        return 1;
    }

    freeaddrinfo(results);

    return 0;
}
