#include "server_message.h"
#include "common_socket.h"
#include "server_dbus_protocol.h"
#include <stdio.h>
#include <stdlib.h>

#define ERROR 1
#define SUCCESS 0

int server_message_create(server_message_t *self, server_info_t *server_info) {
    server_dbus_protocol_t server_dbus_protocol;

    int status = server_dbus_protocol_create(&server_dbus_protocol, 
                                                server_info);

    if (status == ERROR || status == EOF) {
        return status;
    }

    server_dbus_protocol_DBUS_to_message(&server_dbus_protocol, self);

    server_dbus_protocol_destroy(&server_dbus_protocol);
    
    return SUCCESS;
}

int server_message_destroy(server_message_t *self) {
    free(self->message);
    
    return SUCCESS;
}

static void get_word_length(server_message_t *self, uint32_t *word_length, 
                            uint32_t msg_index) {
    while ((self->message)[msg_index] != '\0') {
        (*word_length)++;
        msg_index++;
    }
}

static int get_word(server_message_t *self, char **word, uint32_t *msg_index) {
    uint32_t word_length = 0;
    
    get_word_length(self, &word_length, *msg_index);

    (*word) = calloc(word_length + 1, sizeof(char));

    if (*word == NULL) {
        return ERROR;
    }

    for (uint32_t i = 0; i < word_length; i++) {
        (*word)[i] = (self->message)[*msg_index];
        (*msg_index)++;
    }

    (*word)[word_length] = '\0';
    while (((*msg_index) < self->msg_length) && 
            (self->message[*msg_index] == '\0')) {
        (*msg_index)++;
    }

    return SUCCESS;
}

static void print_word(uint32_t word_number, char *word) {
    switch (word_number) {
    case 0:
        printf("* Destino: %s\n", word);
        break;

    case 1:
        printf("* Ruta: %s\n", word);
        break;

    case 2:
        printf("* Interfaz: %s\n", word);
        break;

    case 3:
        printf("* Metodo: %s\n", word);
        break;

    case 4:
        printf("* Parametros:\n");

    default:
        printf("    * %s\n", word);
        break;
    }
}

int server_message_print(server_message_t *self) {
    uint32_t msg_index = 0;
    uint32_t word_number = 0;
    char *word;

    printf("* Id: 0x%08x\n", self->msg_id);
    
    while (msg_index < (self->msg_length)) {
        if (get_word(self, &word, &msg_index) == ERROR) {
            return ERROR;
        }
        print_word(word_number, word);
        free(word);
        word_number++;
    }

    printf("\n");

    return SUCCESS;
}
