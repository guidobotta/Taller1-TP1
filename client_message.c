#include "client_message.h"
#include "client_msgbuffer.h"
#include "client_dbus_protocol.h"
#include "client_info.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ERROR 1
#define SUCCESS 0

static int get_file(const char *path, FILE **input) {
    (*input) = stdin;
    if (path != NULL) {
        (*input) = fopen(path, "r");
        if ((*input) == NULL) {
            printf("Error: %s\n", strerror(errno));
            return ERROR;
        }
    }
    return SUCCESS;
}

int client_message_create(client_message_t *self, const char *path) {
    if (get_file(path, &(self->input)) == ERROR) {
        return ERROR;
    }

    int status;
    if (client_msgbuffer_create(&(self->client_msgbuffer)) == ERROR) {
        return ERROR;
    }

    self->msgmemory = 64;
    self->message = calloc(self->msgmemory, sizeof(char));
    self->msglenght = 0;
    
    status = client_msgbuffer_getline(&(self->client_msgbuffer), self, self->input);
    
    if ((status == EOF) || (status == ERROR)) {
        return status;
    }

    return SUCCESS;
}

int client_message_destroy(client_message_t *self) {
    if (client_msgbuffer_destroy(&(self->client_msgbuffer)) == ERROR) {
        return ERROR;
    }

    free(self->message);

    if (self->input != stdin) {
        if (fclose(self->input) == EOF){
            printf("Error: %s\n", strerror(errno));
            return ERROR;
        }
    }
    
    return SUCCESS;
}

int client_message_realloc(client_message_t *self) {
    char* result = realloc(self->message, ((self->msgmemory) += 64));

    if (result == NULL){
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    } else {
        self->message = result;
    }

    return SUCCESS;
}
