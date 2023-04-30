#include "networking/logic/network_life_cycle.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_console.hpp"

REFERENCE_DECLARE(0x019AB7F0, s_network_life_cycle_globals, life_cycle_globals);

void __cdecl network_life_cycle_end()
{
    INVOKE(0x00454B40, network_life_cycle_end);
}

void __cdecl network_life_cycle_get_matchmaking_progress(s_life_cycle_matchmaking_progress* progress_out)
{
    INVOKE(0x00454C20, network_life_cycle_get_matchmaking_progress, progress_out);
}

e_life_cycle_state __cdecl network_life_cycle_get_state()
{
    //return INVOKE(0x00454DB0, network_life_cycle_get_state);

    if (life_cycle_globals.initialized)
        return life_cycle_globals.m_state_manager.m_current_state;
    return _life_cycle_state_none;
}

bool __cdecl network_life_cycle_in_squad_session(c_network_session** out_active_squad_session)
{
    return INVOKE(0x00454F20, network_life_cycle_in_squad_session, out_active_squad_session);
}

void __cdecl network_life_cycle_request_leave(bool disconnect)
{
    //return INVOKE(0x00455260, network_life_cycle_request_leave, disconnect);

    life_cycle_globals.m_state_manager.request_leave_sessions(disconnect);
}

bool __cdecl network_life_cycle_set_pre_game_state()
{
    return INVOKE(0x00455280, network_life_cycle_set_pre_game_state);
}

