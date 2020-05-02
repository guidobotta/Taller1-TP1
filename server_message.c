#include "server_message.h"
#include "socket.h"
#include "dbus_protocol_sv.h"
#include <stdio.h>
#include <stdlib.h>

int server_message_create(server_message_t *self, info_server_t *info_server) {
    dbus_protocol_sv_t dbus_protocol_sv;

    dbus_protocol_sv_create(&dbus_protocol_sv, info_server);

    dbus_protocol_sv_DBUS_to_message(&dbus_protocol_sv, self);

    dbus_protocol_sv_destroy(&dbus_protocol_sv);
    
    return 0;
}

int server_message_destroy(server_message_t *self) {
    free(self->message);
    
    return 0;
}

static void get_word_length(server_message_t *self, uint32_t *word_length, uint32_t msg_index) {
    while ((self->message)[msg_index] != '\0') {
        (*word_length)++;
        msg_index++;
    }
}

static void get_word(server_message_t *self, char **word, uint32_t *msg_index) {
    uint32_t word_length = 0;
    
    get_word_length(self, &word_length, *msg_index);

    (*word) = calloc(word_length + 1, sizeof(char));

    for (uint32_t i = 0; i < word_length; i++) {
        (*word)[i] = (self->message)[*msg_index];
        (*msg_index)++;
    }

    (*word)[word_length] = '\0';
    (*msg_index)++;
}

static void print_word(uint32_t word_number, char *word) {
    switch (word_number) {
    case 0:
        printf("* Id: %s\n", word);
        break;
    
    case 1:
        printf("* Destino: %s\n", word);
        break;

    case 2:
        printf("* Path: %s\n", word);
        break;

    case 3:
        printf("* Interfaz: %s\n", word);
        break;

    case 4:
        printf("* Método: %s\n", word);
        break;

    case 5:
        printf("* Parámetros:\n");

    default:
        printf("\t* %s", word);
        break;
    }
}

int server_message_print(server_message_t *self) {
    uint32_t msg_index = 0;
    uint32_t word_number = 0;
    char *word;
    
    while (msg_index < (self->msg_length)) {
        get_word(self, &word, &msg_index);
        print_word(word_number, word);
        free(word);
    }

    return 0;
}
