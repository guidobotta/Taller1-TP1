#include "client_controller.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ERROR 1
#define SUCCESS 0

int main(int argc, char const *argv[]) {
    if ((argc < 3) || (argc > 4)) {
        printf("Error en la cantidad de parámetros.");
        return ERROR;
    }
    
    const char* path = NULL;
    if (argc == 4) {
        path = argv[3];
    }

    client_controller_t client_controller;

    if (client_controller_create(&client_controller, argv[1], argv[2], path)
            == ERROR) {
        return ERROR;
    }

    if (client_controller_execute(&client_controller) == ERROR) {
        client_controller_destroy(&client_controller);
        return ERROR;
    }

    if (client_controller_destroy(&client_controller) == ERROR) {
        return ERROR;
    }
  
    return SUCCESS;
}
