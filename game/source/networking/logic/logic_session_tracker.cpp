#include "networking/logic/logic_session_tracker.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x0229EC20, s_session_tracker_globals, session_tracker_globals);

#define SESSION_STORAGE_COUNT 16

t_value_type<byte> const session_storage_count = { .value = SESSION_STORAGE_COUNT };
t_value_type<dword> const session_storage_size = { .value = sizeof(s_network_session_tracker_session) * SESSION_STORAGE_COUNT };

// c_session_tracker::add_session
DATA_PATCH_DECLARE(0x004E253A + 2, session_storage_count, session_storage_count.bytes); // cmp     esi, 100
DATA_PATCH_DECLARE(0x004E253F + 1, session_storage_count, session_storage_count.bytes); // push    100

// c_session_tracker::allocate_storage
DATA_PATCH_DECLARE(0x004E2676 + 1, session_storage_size, session_storage_size.bytes); // push    8B9340h
DATA_PATCH_DECLARE(0x004E267E + 3, session_storage_size, session_storage_size.bytes); // mov     dword ptr [esi+14h], 8B9340h

//.text:004E2DB0 ; void __cdecl network_session_tracker_clear()
//.text:004E2DC0 ; void __cdecl network_session_tracker_clear_unsuitable_sessions()

void __cdecl network_session_tracker_dispose()
{
    INVOKE(0x004E2DF0, network_session_tracker_dispose);
}

//.text:004E2E40 ; long __cdecl network_session_tracker_get_session_count()
//.text:004E2E50 ; bool __cdecl network_session_tracker_get_session_data(long, s_network_session_tracker_session_data*)
//.text:004E2E70 ; void __cdecl network_session_tracker_get_session_status(long, s_network_session_tracker_session_status*)

bool __cdecl network_session_tracker_initialize()
{
    return INVOKE(0x004E2E90, network_session_tracker_initialize);
}

//.text:004E2EA0 ; bool __cdecl network_session_tracker_mark_session_undesirable(s_transport_session_description const*, e_network_session_tracker_session_undesirable_reason)
//.text:004E2ED0 ; bool __cdecl network_session_tracker_mark_session_unsuitable(s_transport_session_description const*, e_network_session_tracker_session_unsuitable_reason)
//.text:004E2F00 ; bool __cdecl network_session_tracker_session_contacted(long)
//.text:004E2F30 ; bool __cdecl network_session_tracker_start(e_network_session_tracker_sort_method, e_network_session_qos_status_data_type, c_matchmaking_quality*)
//.text:004E2F50 ; void __cdecl network_session_tracker_stop()
//.text:004E2F90 ; bool __cdecl network_session_tracker_track_session(char const*, s_transport_session_description const*)

void __cdecl network_session_tracker_update()
{
    INVOKE(0x004E2FB0, network_session_tracker_update);
}


