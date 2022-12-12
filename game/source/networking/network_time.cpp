#include "networking/network_time.hpp"

#include "cseries/cseries_windows.hpp"
#include "networking/session/network_session_manager.hpp"
#include "networking/transport/transport_security.hpp"

#include <assert.h>

REFERENCE_DECLARE(0x019E8D4C, s_network_time_globals, network_time_globals);

long __cdecl network_time_difference_msec(dword time_a, dword time_b)
{
    return time_b - time_a;
}

dword __cdecl network_time_get()
{
    //return INVOKE(0x00439E40, network_time_get);

    if (network_time_globals.locked)
        return network_time_globals.time;
     
    return network_time_get_exact();
}

dword __cdecl network_time_get_exact()
{
    //return INVOKE(0x00439E60, network_time_get_exact);

    return system_milliseconds();
}

long network_time_since(dword time)
{
    //return INVOKE(0x00439E70, network_time_since, time);

    return network_time_difference_msec(time, network_time_get());
}

void __cdecl network_session_time_deregister_session_manager()
{
    //INVOKE(0x0045F190, network_session_time_deregister_session_manager);

    assert(network_time_globals.session_manager != NULL);

    network_time_globals.session_manager = NULL;
}

bool __cdecl network_session_time_exists(s_transport_secure_identifier const* session_id)
{
    assert(session_id);

    return INVOKE(0x0045F1A0, network_session_time_get_time, session_id);
}

bool __cdecl network_session_time_get_id_and_time(long session_index, s_transport_secure_identifier* session_id, dword* time)
{
    return INVOKE(0x0045F1D0, network_session_time_get_id_and_time, session_index, session_id, time);
}

dword __cdecl network_session_time_get_time(s_transport_secure_identifier const* session_id)
{
    assert(session_id);

    return INVOKE(0x0045F230, network_session_time_get_time, session_id);
}

void __cdecl network_session_time_register_session_manager(c_network_session_manager* session_manager)
{
    //INVOKE(0x0045F260, network_session_time_register_session_manager, session_manager);

    assert(network_time_globals.session_manager == NULL);

    network_time_globals.session_manager = session_manager;
}

void __cdecl network_time_lock(bool lock)
{
    //INVOKE(0x0045F270, network_time_lock, lock);

    network_time_globals.locked = lock;
    network_time_globals.time = network_time_get_exact();
}

