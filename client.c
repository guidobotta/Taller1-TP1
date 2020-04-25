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
    const *FILE input;

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

    if (!status) {
        printf("Error in getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    // Recorro resultados de getaddrinfo
    status = -1;
    addr_ptr = result;
    while ((addr_ptr != NULL) || (status != -1)) {
        if (socket_create(&clsocket) == -1) {
            printf("Error: %s\n", strerror(errno));
        } else {
            status = socket_connect(&clsocket, addr_ptr->ai_addr, addr_ptr->ai_addrlen);
            if (status == -1) {
                socket_destroy(&clsocket);
            }
        }
        addr_ptr = addr_ptr->ai_next;
    }

    return 0;
}
