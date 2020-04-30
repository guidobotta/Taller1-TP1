#ifndef __DBUS_PROTOCOL_CL_H__
#define __DBUS_PROTOCOL_CL_H__

#include <stdint.h>
#include "client_message.h"

typedef struct dbus_protocol_cl {
    char* dbusheader;
    uint32_t header_size;
    uint32_t header_length;
    char* dbusbody;
    uint32_t body_size;
    uint32_t body_length;
} dbus_protocol_cl_t;

int dbus_protocol_cl_create(dbus_protocol_cl_t *self);

int dbus_protocol_cl_destroy(dbus_protocol_cl_t *self);

int dbus_protocol_cl_message_to_DBUS(dbus_protocol_cl_t *self,
                            client_message_t *client_message, uint32_t msg_id);

#endif