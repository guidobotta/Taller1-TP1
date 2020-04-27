#include "socket.h"
#include "info_server.h"

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

    info_server_t info_server;

    if (info_server_create(&info_server, argv[1]) == 1) return 1;
    if (info_server_establish_connection(&info_server) == 1) return 1;
    //HACER COSAS
    if (info_server_destroy(&info_server) == 1) return 1;

    return 0;
}
