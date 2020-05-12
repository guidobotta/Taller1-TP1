#include "server_info.h"
#include "common_socket.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define SOCKET_ERROR -1
#define ERROR 1
#define SUCCESS 0

int server_info_create(server_info_t *self, const char *servicename){
    int status;

    self->servicename = servicename;

    memset(&(self->hints), 0, sizeof(struct addrinfo));
    (self->hints).ai_family = AF_INET;       // IPv4
    (self->hints).ai_socktype = SOCK_STREAM; // TCP
    (self->hints).ai_flags = AI_PASSIVE;     // For bind

    status = getaddrinfo(0, self->servicename, &(self->hints), 
                            &(self->results));

    if (status != 0) {
        printf("Error in getaddrinfo: %s\n", gai_strerror(status));
        return ERROR;
    }

    return SUCCESS;
}

int server_info_destroy(server_info_t *self){
    freeaddrinfo(self->results);
    
    if (socket_shutdown((&self->blsocket), SHUT_RDWR) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    if (socket_shutdown((&self->peersocket), SHUT_RDWR) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    if (socket_destroy((&self->blsocket)) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    if (socket_destroy((&self->peersocket)) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }
    
    return SUCCESS;
}

int server_info_establish_connection(server_info_t *self){
    struct addrinfo *addr_ptr;

    // Recorro resultados de getaddrinfo
    for (addr_ptr = (self->results); addr_ptr != NULL; 
            addr_ptr = addr_ptr->ai_next) {
        if (socket_create(&(self->blsocket)) == SOCKET_ERROR) {
            printf("Error: %s\n", strerror(errno));
        } else {
            if (socket_bind(&(self->blsocket), addr_ptr->ai_addr, 
                                addr_ptr->ai_addrlen) != SOCKET_ERROR) {
                break;
            } else {
                printf("Error: %s\n", strerror(errno));
            }
            socket_destroy(&(self->blsocket));
        }
    }
    
    if (addr_ptr == NULL){
        printf("Error: Could not bind.");
        return ERROR;
    }

    if (socket_listen(&(self->blsocket), 10) == SOCKET_ERROR) { //Aca ojo si el bind se ejecuta exitosamente\
        pero el listen falla después, vas a quedarte sin iterar elementos de la lista. Te recomiendo hacer la sig\
        modificación: if (bind(...) == 0 && listen(...) == 0) break;
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }
    if (socket_accept(&(self->blsocket), addr_ptr->ai_addr, 
            &(addr_ptr->ai_addrlen), &(self->peersocket)) == SOCKET_ERROR) {
        printf("Error: %s\n", strerror(errno));
        return ERROR;
    }

    return SUCCESS;
}

int server_info_send_client_confirmation(server_info_t *self) {

    //No esta tan buena esta función aca, es poco cohesiva en este TDA\
    Estas añadiéndole una responsabilidad extra de la cual no debería\
    hacerse cargo. Este TDA deberia proveer una api que permita simplemente\
    enviar y recibir mensajes, y desde afuera hacer algo del estilo\
    server_info_send_message(msg). Fijate que haciendolo de esta forma tenes\
    un socket funcional mucho mas generico y que permite enviar CUALQUIER mensaje.

    //Otra opción (un poco más amigable): lo que hacen tus funciones\
    server_infoestablish_connection y server_info_create encapsularlas en tu TDA\
    common_socket y lo unico que terminarias haciendo en el TDA server_info y en client_info\
    es wrappear todas las funciones de common_socket para facilmente crear un socket\
    cliente o servidor.

    //Oootra opción (la que considero ideal): Tener un único TDA para manejar a los sockets\
    (es decir, darle todo el peso a tu TDA common_socket) y proveer una interfaz que permita:\
    crear un client-socket o un server-socket y enviar o recibir mensajes.


    char* confirmation = "OK";

    if (socket_send(&(self->peersocket), confirmation, 3, 0) == -1) {
        return ERROR;
    }

    return SUCCESS;
}
