#include "client_info.h"
#include "common_socket.h"
#include "client_message.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ERROR 1
#define SUCCESS 0

int send_recieve_to_server(FILE* input, client_info_t *client_info){
    client_message_t client_message;
    int status;
    uint32_t msg_id = 1;

    while ((status = client_message_create(&client_message, input)) != EOF) {
        if (status == ERROR) {
            return ERROR;
        }

        if (client_message_send(&client_message, client_info, msg_id) == ERROR){
            client_message_destroy(&client_message);
            return ERROR;
        }

        if (client_info_recibe_confirmation(client_info, msg_id) == ERROR) {
            client_message_destroy(&client_message);
            return ERROR;
        }

        if (client_message_destroy(&client_message) == ERROR) {
            return ERROR;
        }

        msg_id++;
    }

    if (client_message_destroy(&client_message) == ERROR) {
        return ERROR;
    }

    return SUCCESS;
}

int communicate_to_server(int argc, char const *argv[], 
                            client_info_t *client_info){
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

    if (send_recieve_to_server(input, client_info) == ERROR){
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

    return SUCCESS;
}

int main(int argc, char const *argv[]) {
    if ((argc < 3) || (argc > 4)) {
        printf("Error en la cantidad de parámetros.");
        return ERROR;
    }

    client_info_t client_info;

    if (client_info_create(&client_info, argv[1], argv[2]) == ERROR) {
        return ERROR;
    }

    if (client_info_establish_connection(&client_info) == ERROR) {
        client_info_destroy(&client_info);
        return ERROR;
    }

    if (communicate_to_server(argc, argv, &client_info) == ERROR) {
        client_info_destroy(&client_info);
        return ERROR;
    }

    if (client_info_destroy(&client_info) == ERROR) {
        return ERROR;
    }

    return SUCCESS;
}
