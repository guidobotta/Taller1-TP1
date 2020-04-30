#include "client_message.h"
#include "dbus_protocol_cl.h"
#include <stdlib.h>
#include <stdint.h>

int dbus_protocol_cl_create(dbus_protocol_cl_t *self) {
    (self->header_length) = 0;
    (self->header_size) = 128;
    (self->dbusheader) = calloc(self->header_size, sizeof(char));

    (self->body_length) = 0;
    (self->body_size) = 4;
    (self->dbusbody) = (char *) calloc(self->body_size, sizeof(char));

    return 0;
}

int dbus_protocol_cl_destroy(dbus_protocol_cl_t *self) {
    free(self->dbusheader);
    free(self->dbusbody);

    return 0;
}

static void get_word_length(client_message_t *client_message, uint32_t index,
                     uint32_t *word_length, char delimiter){
    while ((client_message->message)[index] != delimiter) {
        (*word_length)++;
        index++;
    }
}

static void set_int32(uint32_t intc, char** buffer, uint32_t *index) {
    unsigned char cint[4];

    cint[0] = (unsigned char)(intc & 0xFF);
    cint[1] = (unsigned char)((intc >> 8) & 0xFF);
    cint[2] = (unsigned char)((intc >> 16) & 0xFF);
    cint[3] = (unsigned char)((intc >> 24) & 0xFF);

    for (int i = 0; i < 4; i++) {
        (*buffer)[*index] = cint[i];
        (*index)++;
    }
}

static void set_parameter(dbus_protocol_cl_t *self, 
client_message_t *client_message, uint32_t *body_index, uint32_t *msg_index) {
    uint32_t word_length = 0;
    get_word_length(client_message, *msg_index, &word_length, ',');

    (self->dbusbody) = realloc(self->dbusbody, 
                                ((self->body_size) += word_length + 4));
    
    set_int32(word_length, &(self->dbusbody), body_index);

    for (int i = 0; i < word_length; i++) {
        (self->dbusbody)[*body_index] = (client_message->message)[*msg_index];
        (*msg_index)++;
        (*body_index)++;
    }

    (self->dbusbody)[*body_index] = 0;
    (*msg_index)++;
    (*body_index)++;
}

static void set_four_chars(dbus_protocol_cl_t *self, uint32_t *header_index, 
                            char c[4]) {
    for (int i = 0; i < 4; i++) {
        (self->dbusheader)[*header_index] = c[i];
        (*header_index)++;
    }
}

static void set_param_data(dbus_protocol_cl_t *self, uint32_t *header_index, 
                            int wordnumber) {
    switch (wordnumber) {
    case 0: ;
        char c1[4] = {6, 1, 'o', 0};
        set_four_chars(self, header_index, c1);
        break;
    
    case 1: ;
        char c2[4] = {1, 1, 's', 0};
        set_four_chars(self, header_index, c2);
        break;

    case 2: ;
        char c3[4] = {2, 1, 's', 0};
        set_four_chars(self, header_index, c3);
        break;

    case 3: ;
        char c4[4] = {3, 1, 's', 0};
        set_four_chars(self, header_index, c4);
        break;

    default: ;
        char c5[4] = {9, 1, 'g', 0};
        set_four_chars(self, header_index, c5);
        break;
    }

}

static void set_word(client_message_t *client_message, dbus_protocol_cl_t *self,
        uint32_t *header_index, uint32_t *msg_index, uint32_t *word_length) {
    while (client_message->message[*msg_index] != ' ') {
        (self->dbusheader)[*header_index] = client_message->message[*msg_index];
        (*header_index)++;
        (*msg_index)++;
        (*word_length)++;
    }
}

static void set_padding(dbus_protocol_cl_t *self, uint32_t *header_index,
                        uint32_t *word_length) {
    do {
        (self->dbusheader)[*header_index] = 0;
        (*header_index)++;
        (*word_length)++;
    } while ((*word_length) % 8 == 0);
}

static void check_header_mem(dbus_protocol_cl_t *self, uint32_t header_index, 
                                uint32_t word_length, int wordnumber) {
    if (wordnumber < 4) {
        if (header_index + word_length + 16 > (self->header_size)) {
            (self->dbusheader) = realloc(self->dbusheader, 
                                        ((self->header_size) *= 2));
        }
    } else {
        if (header_index + 16 > (self->header_size)) {
            (self->dbusheader) = realloc(self->dbusheader, 
                                        ((self->header_size) += 16));
        }
    }
}

static void set_array(client_message_t *client_message, dbus_protocol_cl_t *self,
                        int wordnumber, uint32_t *header_index, 
                        uint32_t *msg_index, uint32_t *array_length) {
    uint32_t word_length = 0;

    get_word_length(client_message, *msg_index, &word_length, ' ');

    check_header_mem(self, *header_index, word_length, wordnumber);

    set_param_data(self, header_index, wordnumber);

    set_int32(word_length, &(self->dbusheader), header_index);

    (*array_length) += 8;

    if (wordnumber < 4) {
        set_word(client_message, self, header_index, msg_index, 
                    &word_length);
        set_padding(self, header_index, &word_length);
        (*array_length) += word_length;                    // VER TEMA DE NO SUMAR EL ULTIMO PADDING
    }
}

static uint32_t get_body(dbus_protocol_cl_t *self, 
                            client_message_t *client_message) {
    uint32_t msg_index = 0;
    uint32_t body_index = 0;

    while ((client_message->message)[msg_index] != '(') msg_index++;

    msg_index++;

    while ((client_message->message)[msg_index] != ')') {
        set_parameter(self, client_message, &body_index, &msg_index);
    }

    return body_index;
}

static uint32_t get_header(dbus_protocol_cl_t *self, 
                        client_message_t *client_message, uint32_t msg_id) {
    uint32_t header_index = 0;

    char c[4] = {'l', 1, 0, 1};
    set_four_chars(self, &header_index, c);

    set_int32((self->body_size), &(self->dbusheader), &header_index);

    set_int32(msg_id, &(self->dbusheader), &header_index);

    uint32_t array_length = 0;
    uint32_t array_length_index = header_index;

    header_index += 4;

    uint32_t msg_index = 0;

    for (int wordnumber = 0; (client_message->message)[msg_index] != '('; 
            wordnumber++) {
        set_array(client_message, self, wordnumber, &header_index, &msg_index, 
                    &array_length);
    }

    set_int32(array_length, &(self->dbusheader), &array_length_index);

    return header_index;
}

int dbus_protocol_cl_message_to_DBUS(dbus_protocol_cl_t *self, 
                        client_message_t *client_message, uint32_t msg_id) {
    
    (self->body_length) = get_body(self, client_message);

    (self->header_length) = get_header(self, client_message, msg_id);

    return 0;
}