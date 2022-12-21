#include "networking/session/network_managed_session.hpp"

REFERENCE_DECLARE(0x02247448, s_online_session_manager_globals, online_session_manager_globals);

char const* __cdecl managed_session_get_id_string(long managed_session_index)
{
    //INVOKE(0x004820A0, managed_session_get_id_string, managed_session_index);

    if (managed_session_index == -1)
        return "00:00:00:00:00:00:00:00";
    else
        return transport_secure_identifier_get_string(&online_session_manager_globals.managed_sessions[managed_session_index].actual_online_session_state.description.id);
}

