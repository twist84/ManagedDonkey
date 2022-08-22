#include "networking/network_life_cycle.hpp"

long __cdecl network_life_cycle_get_state()
{
    return reinterpret_cast<decltype(network_life_cycle_get_state)*>(0x00454DB0)();
}

void __cdecl network_life_cycle_request_leave(bool disconnect)
{
    reinterpret_cast<decltype(network_life_cycle_request_leave)*>(0x00455260)(disconnect);
}

void __cdecl network_life_cycle_end()
{
    return reinterpret_cast<decltype(network_life_cycle_end)*>(0x00454B40)();
}
