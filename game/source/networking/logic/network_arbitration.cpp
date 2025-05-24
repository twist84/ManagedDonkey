#include "networking/logic/network_arbitration.hpp"

#include "memory/module.hpp"

REFERENCE_DECLARE(0x0228E520, s_network_arbitration_globals, network_arbitration_globals);

HOOK_DECLARE(0x004D9B60, network_arbitration_initiate);

void __cdecl network_arbitration_destory()
{
	INVOKE(0x004D9A90, network_arbitration_destory);
}

//.text:004D9AB0 ; bool __cdecl network_arbitration_get_results(int32, s_arbitration_registration_result*)
//.text:004D9B20 ; e_network_arbitration_status __cdecl network_arbitration_get_status()

bool __cdecl network_arbitration_initialize()
{
	return INVOKE(0x004D9B30, network_arbitration_initialize);
}

bool __cdecl network_arbitration_initiate(int32 managed_session_index, uns64 nonce)
{
	//return INVOKE(0x004D9B60, network_arbitration_initiate, managed_session_index, nonce);

	network_arbitration_globals.arbitration_status = _network_arbitration_status_registered;
	network_arbitration_globals.managed_session_index = managed_session_index;
	return true;
}

void __cdecl network_arbitration_update()
{
	INVOKE(0x004D9BB0, network_arbitration_update);
}

//.text:004D9BE0 ; void __cdecl network_arbitration_update_registration()

