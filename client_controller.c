#include "client_controller.h"
#include <errno.h>
#include <string.h>

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

int client_controller_create(client_controller_t *self, const char *hostname, 
                        const char *servicename, const char *path) {
    if (client_info_create(&(self->client_info), hostname, servicename) == 
            ERROR) {
        return ERROR;
    }

    if (get_file(path, &(self->input)) == ERROR) {
        client_info_destroy(&(self->client_info));
        return ERROR;
    }
    
    self->msg_id = 1;
    if ((self->status = client_message_create(&(self->client_message), 
            self->input)) == ERROR) {
        client_info_destroy(&(self->client_info));
        return ERROR;
    }

    return SUCCESS;
}

int client_controller_destroy(client_controller_t *self) {
    if (self->input != stdin) {
        if (fclose(self->input) == EOF) {
            client_message_destroy(&(self->client_message));
            client_info_destroy(&(self->client_info));
            return ERROR;
        }
    }
    
    if (client_message_destroy(&(self->client_message)) == ERROR) {
        client_info_destroy(&(self->client_info));
        return ERROR;
    }

    if (client_info_destroy(&(self->client_info)) == ERROR) {
        return ERROR;
    }

    return SUCCESS;
}

static int receive_confirmation(client_controller_t *self) {
    char buff[3];

    if (client_info_recibe_message(&(self->client_info), buff, 3) == ERROR) {
        return ERROR;
    }

    printf("0x%08x: %s\n", self->msg_id, buff);

    return SUCCESS;
}

int client_controller_execute(client_controller_t *self) {
    if (client_info_establish_connection(&(self->client_info)) == ERROR) {
        return ERROR;
    }

    while (self->status != EOF) {
        if (self->status == ERROR) {
            return ERROR;
        }

        if (client_info_send_message(&(self->client_info), 
            &(self->client_message), self->msg_id) == ERROR){
            return ERROR;
        }

        if (receive_confirmation(self) == ERROR) {
            return ERROR;
        }

        if (client_message_destroy(&(self->client_message)) == ERROR) {
            return ERROR;
        }

        if ((self->status = client_message_create(&(self->client_message), 
            self->input)) == ERROR) {
            return ERROR;
        }

        (self->msg_id)++;
    }

    return SUCCESS;
}
