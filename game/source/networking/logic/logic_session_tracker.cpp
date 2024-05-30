#include "networking/logic/logic_session_tracker.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

t_value_type<dword> const session_storage_size = { .value = 0x16550 * 16 };

// c_session_tracker::allocate_storage
DATA_PATCH_DECLARE(0x004E2676 + 1, session_storage_size, session_storage_size.bytes); // push    8B9340h
DATA_PATCH_DECLARE(0x004E267E + 3, session_storage_size, session_storage_size.bytes); // mov     dword ptr [esi+14h], 8B9340h

bool __cdecl network_session_tracker_initialize()
{
    return INVOKE(0x004E2E90, network_session_tracker_initialize);
}

