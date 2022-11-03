#pragma once

#include "networking/transport/transport_security.hpp"

struct s_xnet_entry
{
    bool initialized;
    byte : 8;
    byte : 8;
    byte : 8;

    transport_address address;
    s_transport_secure_address secure_address;
    s_transport_secure_identifier secure_identifier;
};
static_assert(sizeof(s_xnet_entry) == 0x38);

extern s_xnet_entry(&xnet_mapping)[51];

