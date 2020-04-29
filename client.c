#include "info_client.h"
#include "socket.h"
#include "client_message.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ERROR 1
#define SUCCESS 0

int send_recieve_to_server(FILE* input){
    client_message_t client_message;
    int status;
    size_t id = 0;    

    while ((status = client_message_create(&client_message, input)) != EOF) {
        if (status == ERROR) {
            return ERROR;
        }

        if (client_message_to_DBUS(&client_message, id) == ERROR){
            client_message_destroy(&client_message);
            return ERROR;
        }

        id++;

        //ENVIAR, RECIBIR E IMPRIMIR

        if (client_message_destroy(&client_message) == ERROR) {
            return ERROR;
        }
    }

    return SUCCESS;
}

int communicate_to_server(int argc, char const *argv[]){
    FILE *input;
    
    if (argc == 4) {
        input = fopen(argv[3], "r");
    } else {
        input = stdin;
    }

    if (input == NULL){
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    if (send_recieve_to_server(input) == ERROR){
        if (argc == 4) {
            if (fclose(input) == EOF){
                printf("Error: %s\n", strerror(errno));
            }
        }
        return ERROR;
    }

    if (argc == 4) {
        if (fclose(input) == EOF){
            printf("Error: %s\n", strerror(errno));
            return ERROR;
        }
    }
}

int main(int argc, char const *argv[]) {
    if ((argc < 3) || (argc > 4)) {
        printf("Error en la cantidad de parámetros.");
        return ERROR;
    }

    info_client_t info_client;

    if (info_client_create(&info_client, argv[1], argv[2]) == ERROR) {
        return ERROR;
    }

    if (info_client_establish_connection(&info_client) == ERROR) {
        info_client_destroy(&info_client);
        return ERROR;
    }

    if (communicate_to_server(argc, argv) == ERROR) {
        info_client_destroy(&info_client);
        return ERROR;
    }

    if (info_client_destroy(&info_client) == ERROR) {
        return ERROR;
    }

    return SUCCESS;
}
