#include "networking/delivery/network_link.hpp"

#include "networking/transport/transport.hpp"

bool __cdecl c_network_link::physical_link_available()
{
    return transport_available();
}

