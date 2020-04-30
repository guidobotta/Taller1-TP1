#ifndef __DBUS_PROTOCOL_SV_H__
#define __DBUS_PROTOCOL_SV_H__

#include <stdint.h>

typedef struct dbus_protocol_sv {
    char* dbusheader;
    uint32_t header_length;
    char* dbusbody;
    uint32_t body_length;
} dbus_protocol_sv_t;

int dbus_protocol_sv_create(dbus_protocol_sv_t *self, uint32_t body_length, 
                            uint32_t array_length);

int dbus_protocol_sv_destroy(dbus_protocol_sv_t *self);

int dbus_protocol_sv_DBUS_to_message(dbus_protocol_sv_t *self);

#endif