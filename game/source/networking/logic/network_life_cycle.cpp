#include "networking/logic/network_life_cycle.hpp"

s_network_life_cycle_globals& life_cycle_globals = *reinterpret_cast<s_network_life_cycle_globals*>(0x019AB7F0);

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
