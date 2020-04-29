#include "client_message.h"
#include "msgbuffer.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ERROR 1
#define SUCCESS 0

int client_message_create(client_message_t *self, FILE *input) {
    int status;
    msgbuffer_create(&(self->msgbuffer));

    self->msgmemory = 64;
    self->message = calloc(self->msgmemory, sizeof(char));
    self->msglenght = 0;
    
    status = msgbuffer_getline(&(self->msgbuffer), self, input);
    
    if ((status == EOF) || (status == ERROR)) {
        return status;
    }

    return SUCCESS;
}

int client_message_destroy(client_message_t *self) {
    msgbuffer_destroy(&(self->msgbuffer));
    free(self->message);
    return SUCCESS;
}

int client_message_realloc(client_message_t *self) {
    self->message = realloc(self->message, ((self->msgmemory) += 64));
    if (self->message == NULL){
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }
    return SUCCESS;
}

void _get_param_lenght(client_message_t *self, size_t index, size_t *word_lenght){
    while ((self->message)[index] != ',') {
        (*word_lenght)++;
        index++;
    }
}

size_t _get_body(client_message_t *self, char** dbusbody){
    size_t mindex = 0;
    size_t bindex = 0;
    size_t word_number = 0;
    size_t word_length = 0;
    unsigned char word_length_bytes[4];
    size_t body_lenght = 4;
    *dbusbody = (char *) calloc(body_lenght, sizeof(char));

    word_length_bytes[0] = word_length & 0xFF;
    word_length_bytes[1] = (word_length >> 8) & 0xFF;
    word_length_bytes[2] = (word_length >> 16) & 0xFF;
    word_length_bytes[3] = (word_length >> 24) & 0xFF;

    for (int i = 0; i < 4; i++) {
        (*dbusbody)[bindex] = word_length_bytes[i];
    }

    while ((self->message)[mindex] != '(') {
        mindex++;
    }

    mindex++;

    while ((self->message)[mindex] != ')') {
        _get_param_lenght(self, mindex, &word_length);
        *dbusbody = realloc(*dbusbody, (body_lenght += word_length + 4));

        word_length_bytes[0] = word_length & 0xFF;
        word_length_bytes[1] = (word_length >> 8) & 0xFF;
        word_length_bytes[2] = (word_length >> 16) & 0xFF;
        word_length_bytes[3] = (word_length >> 24) & 0xFF;

        for (int i = 0; i < 4; i++) {
            (*dbusbody)[bindex] = word_length_bytes[i];
        }

        for (int i = 0; i < word_length; i++) {
            (*dbusbody)[bindex] = (self->message)[mindex];
            mindex++;
            bindex++;
        }

        (*dbusbody)[bindex] = 0;
        mindex++;
        bindex++;

        word_length = 0;
    }

    return body_lenght;
}

size_t _get_word_length(client_message_t *self, size_t msgindex, size_t *word_lenght) {
    while ((self->message)[msgindex] != ' ') {
        (*word_lenght)++;
        msgindex++;
    }
}

void _setword(client_message_t *self, char** dbusheader, int wordnumber, 
                size_t *dbhindex, size_t *msgindex, size_t *header_size) {

    size_t word_length = 0;
    unsigned char word_length_bytes[4];
    
    switch (wordnumber) {
    case 0:
        word_length = _get_word_length(self, *msgindex, &word_length);

        if (*dbhindex + word_length + 16 > *header_size) {
            *dbusheader = realloc(*dbusheader, ((*header_size) *= 2));
        }

        (*dbusheader)[*dbhindex] = 6;
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 1;
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 'o';
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 0;
        (*dbhindex)++;

        word_length_bytes[0] = word_length & 0xFF;
        word_length_bytes[1] = (word_length >> 8) & 0xFF;
        word_length_bytes[2] = (word_length >> 16) & 0xFF;
        word_length_bytes[3] = (word_length >> 24) & 0xFF;

        for (int i = 0; i < 4; i++) {
            (*dbusheader)[*dbhindex] = word_length_bytes[i];
            dbhindex++;
        }

        while (self->message[*msgindex] != ' ') {
            (*dbusheader)[*dbhindex] = self->message[*msgindex];
            (*dbhindex)++;
            (*msgindex)++;
            word_length++;
        }

        do {
            (*dbusheader)[*dbhindex] = 0;
            dbhindex++;
            word_length++;
        } while (word_length % 8 == 0);

        break;
    
    case 1:
        word_length = _get_word_length(self, *msgindex, &word_length);

        if (*dbhindex + word_length + 16 > *header_size) {
            *dbusheader = realloc(*dbusheader, ((*header_size) *= 2));
        }
        
        (*dbusheader)[*dbhindex] = 1;
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 1;
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 's';
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 0;
        (*dbhindex)++;

        word_length_bytes[0] = word_length & 0xFF;
        word_length_bytes[1] = (word_length >> 8) & 0xFF;
        word_length_bytes[2] = (word_length >> 16) & 0xFF;
        word_length_bytes[3] = (word_length >> 24) & 0xFF;

        for (int i = 0; i < 4; i++) {
            (*dbusheader)[*dbhindex] = word_length_bytes[i];
            dbhindex++;
        }

        while (self->message[*msgindex] != ' ') {
            (*dbusheader)[*dbhindex] = self->message[*msgindex];
            dbhindex++;
            (*msgindex)++;
            word_length++;
        }

        do {
            (*dbusheader)[*dbhindex] = 0;
            dbhindex++;
            word_length++;
        } while (word_length % 8 == 0);

        break;

    case 2:
        word_length = _get_word_length(self, *msgindex, &word_length);

        if (*dbhindex + word_length + 16 > *header_size) {
            *dbusheader = realloc(*dbusheader, ((*header_size) *= 2));
        }
        
        (*dbusheader)[*dbhindex] = 2;
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 1;
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 's';
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 0;
        (*dbhindex)++;

        word_length_bytes[0] = word_length & 0xFF;
        word_length_bytes[1] = (word_length >> 8) & 0xFF;
        word_length_bytes[2] = (word_length >> 16) & 0xFF;
        word_length_bytes[3] = (word_length >> 24) & 0xFF;

        for (int i = 0; i < 4; i++) {
            (*dbusheader)[*dbhindex] = word_length_bytes[i];
            dbhindex++;
        }

        while (self->message[*msgindex] != ' ') {
            (*dbusheader)[*dbhindex] = self->message[*msgindex];
            dbhindex++;
            (*msgindex)++;
            word_length++;
        }

        do {
            (*dbusheader)[*dbhindex] = 0;
            dbhindex++;
            word_length++;
        } while (word_length % 8 == 0);
        
        break;

    case 3:
        word_length = _get_word_length(self, *msgindex, &word_length);

        if (*dbhindex + word_length + 16 > *header_size) {
            *dbusheader = realloc(*dbusheader, ((*header_size) *= 2));
        }
        
        (*dbusheader)[*dbhindex] = 3;
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 1;
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 's';
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 0;
        (*dbhindex)++;

        word_length_bytes[0] = word_length & 0xFF;
        word_length_bytes[1] = (word_length >> 8) & 0xFF;
        word_length_bytes[2] = (word_length >> 16) & 0xFF;
        word_length_bytes[3] = (word_length >> 24) & 0xFF;

        for (int i = 0; i < 4; i++) {
            (*dbusheader)[*dbhindex] = word_length_bytes[i];
            dbhindex++;
        }
        
        while (self->message[*msgindex] != ' ') {
            (*dbusheader)[*dbhindex] = self->message[*msgindex];
            dbhindex++;
            (*msgindex)++;
            word_length++;
        }

        do {
            (*dbusheader)[*dbhindex] = 0;
            dbhindex++;
            word_length++;
        } while (word_length % 8 == 0);

        break;

    default:
        word_length = _get_word_length(self, *msgindex, &word_length);

        if ((*dbhindex) + 16 > (*header_size)) {
            *dbusheader = realloc(*dbusheader, ((*header_size) *= 2));
        }
        
        (*dbusheader)[*dbhindex] = 9;
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 1;
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 'g';
        (*dbhindex)++;
        (*dbusheader)[*dbhindex] = 0;
        (*dbhindex)++;

        unsigned char word_length_bytes[4];
        word_length_bytes[0] = word_length & 0xFF;
        word_length_bytes[1] = (word_length >> 8) & 0xFF;
        word_length_bytes[2] = (word_length >> 16) & 0xFF;
        word_length_bytes[3] = (word_length >> 24) & 0xFF;

        for (int i = 0; i < 4; i++) {
            (*dbusheader)[*dbhindex] = word_length_bytes[i];
            dbhindex++;
        }
        
        break;
    }
}

int client_message_to_DBUS(client_message_t *self, size_t msg_id) {
    char *dbusbody;
    size_t body_length = _get_body(self, &dbusbody);

    size_t dbhindex = 0;
    size_t header_size = 128;
    char *dbusheader = calloc(header_size, sizeof(char));

    dbusheader[dbhindex] = 'l';
    dbhindex++;
    dbusheader[dbhindex] = 1;
    dbhindex++;
    dbusheader[dbhindex] = 0;
    dbhindex++;
    dbusheader[dbhindex] = 1;
    dbhindex++;

    unsigned char body_length_bytes[4];
    body_length_bytes[0] = body_length & 0xFF;
    body_length_bytes[1] = (body_length >> 8) & 0xFF;
    body_length_bytes[2] = (body_length >> 16) & 0xFF;
    body_length_bytes[3] = (body_length >> 24) & 0xFF;
    
    for (int i = 0; i < 4; i++) { // 1° int
        dbusheader[dbhindex] = body_length_bytes[i];
        dbhindex++;
    }

    unsigned char msg_id_bytes[4];
    msg_id_bytes[0] = msg_id & 0xFF;
    msg_id_bytes[1] = (msg_id >> 8) & 0xFF;
    msg_id_bytes[2] = (msg_id >> 16) & 0xFF;
    msg_id_bytes[3] = (msg_id >> 24) & 0xFF;

    for (int i = 0; i < 4; i++) { // 2° int
        dbusheader[dbhindex] = msg_id_bytes[i];
        dbhindex++;
    }

    size_t msgindex = 0;

    for (int wordnumber = 0; (self->message)[msgindex] != '('; wordnumber++) {
        _setword(self, &dbusheader, wordnumber, &dbhindex, &msgindex, &header_size);
    }

}