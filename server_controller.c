#include "server_controller.h"
#include "stdio.h"

#define SOCKET_ERROR -1
#define ERROR 1
#define SUCCESS 0

int server_controller_create(server_controller_t *self, 
                            const char *servicename) {
    if (server_info_create(&(self->server_info), servicename) == ERROR) {
        return ERROR;
    }

    return SUCCESS;
}

int server_controller_destroy(server_controller_t *self) {
    if (self->status != EOF) {
        if (server_message_destroy(&(self->server_message)) == ERROR) {
            server_info_destroy(&(self->server_info));
            return ERROR;
        }
    }
    
    if (server_info_destroy(&(self->server_info)) == ERROR) {
        return ERROR;
    }

    return SUCCESS;
}

int server_controller_execute(server_controller_t *self) {
    if (server_info_establish_connection(&(self->server_info)) == ERROR) {
        return ERROR;
    }

    (self->status) = server_message_create(&(self->server_message), 
                                            &(self->server_info));

    while ((self->status) != EOF) {
        if ((self->status) == ERROR) {
            return ERROR;
        }

        if (server_message_print(&(self->server_message)) == ERROR) {
            return ERROR;
        }

        if (server_message_destroy(&(self->server_message)) == ERROR) {
            return ERROR;
        }
        
        if (server_info_send_message(&(self->server_info), "OK", 3) == 
                SOCKET_ERROR) {
            return ERROR;
        }

        (self->status) = server_message_create(&(self->server_message), 
                                                &(self->server_info));
    }

    return SUCCESS;
}
