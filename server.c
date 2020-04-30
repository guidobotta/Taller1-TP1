#include "socket.h"
#include "info_server.h"
#include "server_message.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define ERROR 1
#define SUCCESS 0

int communicate_with_client(info_server_t *info_server) {
    
    server_message_t server_message;

    server_message_create(&server_message, info_server);
    
    server_message_print(&server_message);
    
    server_message_destroy(&server_message);
    
    //enviar ok

    return SUCCESS;
}

int main(int argc, char const *argv[]) {
    
    if (argc != 2) {
        printf("Error en la cantidad de parámetros.");
        return ERROR;
    }

    info_server_t info_server;

    if (info_server_create(&info_server, argv[1]) == ERROR) return ERROR;
    
    if (info_server_establish_connection(&info_server) == ERROR) return ERROR;

    if (communicate_with_client(&info_server) == ERROR) return ERROR;
    
    if (info_server_destroy(&info_server) == ERROR) return ERROR;

    return SUCCESS;
}
