#include "common_socket.h"
#include "server_info.h"
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

int communicate_with_client(server_info_t *server_info) {
    server_message_t server_message;

    int status = server_message_create(&server_message, server_info);

    while (status != EOF) {
        if (status == ERROR) {
            return ERROR;
        }

        if (server_message_print(&server_message) == ERROR) {
            server_message_destroy(&server_message);
            return ERROR;
        }

        if (server_message_destroy(&server_message) == ERROR) {
            return ERROR;
        }
        
        if (server_info_send_client_confirmation(server_info) == ERROR) {
            return ERROR;
        }

        status = server_message_create(&server_message, server_info);
    }

    return SUCCESS;
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("Error en la cantidad de parámetros.");
        return ERROR;
    }

    server_info_t server_info;

    if (server_info_create(&server_info, argv[1]) == ERROR) {
        return ERROR;
    }
    
    if (server_info_establish_connection(&server_info) == ERROR) {
        server_info_destroy(&server_info);
        return ERROR;
    }

    if (communicate_with_client(&server_info) == ERROR) {
        server_info_destroy(&server_info);
        return ERROR;
    }
    
    if (server_info_destroy(&server_info) == ERROR) {
        return ERROR;
    }

    return SUCCESS;
}
