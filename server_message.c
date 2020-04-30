#include "server_message.h"
#include "socket.h"
#include "dbus_protocol_sv.h"

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

int server_message_print(server_message_t *self) {
    
    return 0;
}
