#include "networking/logic/network_life_cycle.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x019AB7F0, s_network_life_cycle_globals, life_cycle_globals);

e_life_cycle_state __cdecl network_life_cycle_get_state()
{
    FUNCTION_BEGIN(true);

    //return INVOKE(0x00454DB0, network_life_cycle_get_state);

    if (life_cycle_globals.initialized)
        return life_cycle_globals.m_state_manager.m_current_state;
    return _life_cycle_state_none;
}

void __cdecl network_life_cycle_request_leave(bool disconnect)
{
    FUNCTION_BEGIN(true);

    //return INVOKE(0x00455260, network_life_cycle_request_leave, disconnect);

    life_cycle_globals.m_state_manager.request_leave_sessions(disconnect);
}

void __cdecl network_life_cycle_end()
{
    FUNCTION_BEGIN(true);

    INVOKE(0x00454B40, network_life_cycle_end);
}

bool __cdecl network_life_cycle_in_squad_session(c_network_session** out_active_squad_session)
{
    FUNCTION_BEGIN(true);

    return INVOKE(0x00454F20, network_life_cycle_in_squad_session, out_active_squad_session);
}
