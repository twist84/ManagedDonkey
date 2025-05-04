#include "networking/logic/network_arbitration.hpp"

REFERENCE_DECLARE(0x0228E520, s_network_arbitration_globals, network_arbitration_globals);

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

//.text:004D9B60 ; bool __cdecl network_arbitration_initiate(int32, uns64)

void __cdecl network_arbitration_update()
{
	INVOKE(0x004D9BB0, network_arbitration_update);
}

//.text:004D9BE0 ; void __cdecl network_arbitration_update_registration()

