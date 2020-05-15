#include "server_controller.h"

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

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("Error en la cantidad de parámetros.");
        return ERROR;
    }

    server_controller_t server_controller;

    if (server_controller_create(&server_controller, argv[1]) == ERROR) {
        return ERROR;
    }

    if (server_controller_execute(&server_controller) == ERROR) {
        return ERROR;
    }

    if (server_controller_destroy(&server_controller) == ERROR) {
        return ERROR;
    }

    return SUCCESS;
}
