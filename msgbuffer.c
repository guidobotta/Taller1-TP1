#include "client_message.h"
#include "msgbuffer.h"
#include <stdio.h>
#include <string.h>

int _fgets32(FILE *input, char buff[]){
    char c = getc(input);
    
    if (c == EOF) return -1;

    int i = 0;
    for(;(i<32) && (c != EOF) && (c != '\n'); i++) {
        buff[i] = c;
        c = getc(input);
    }

    return i;
}

int msgbuffer_create(msgbuffer_t *self){
    (self->buffer)[0] = '\0';
}

int msgbuffer_destroy(msgbuffer_t *self){
    return 0;
}

int msgbuffer_getline(msgbuffer_t *self, client_message_t *msg, FILE *input){
    int n;
    if ((n = _fgets32(input, self->buffer)) == EOF) return EOF;

    while (n == 32){
        if ((n + strlen(msg->message)) > (msg->msgmemory)) {
            client_message_realloc(msg);
        }
        strncat(msg->message, self->buffer, n);
        n = _fgets32(input, self->buffer);
    }

    if ((n + strlen(msg->message)) > (msg->msgmemory)) {
        client_message_realloc(msg);
    }

    strncat(msg->message, self->buffer, n);
    return 0;
}