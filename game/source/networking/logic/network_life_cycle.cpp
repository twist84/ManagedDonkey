#include "networking/logic/network_life_cycle.hpp"

#include "cseries/cseries.hpp"

s_network_life_cycle_globals& life_cycle_globals = *reinterpret_cast<s_network_life_cycle_globals*>(0x019AB7F0);

e_life_cycle_state __cdecl network_life_cycle_get_state()
{
    if (life_cycle_globals.initialized)
        return life_cycle_globals.m_state_manager.m_current_state;
    return _life_cycle_state_none;

    //return DECLTHUNK(0x00454DB0, network_life_cycle_get_state)();
}

void __cdecl network_life_cycle_request_leave(bool disconnect)
{
    life_cycle_globals.m_state_manager.request_leave_sessions(disconnect);

    //return DECLTHUNK(0x00455260, network_life_cycle_request_leave)(disconnect);
}

void __cdecl network_life_cycle_end()
{
    DECLTHUNK(0x00454B40, network_life_cycle_end)();
}
