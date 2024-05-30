#include "networking/logic/network_arbitration.hpp"

REFERENCE_DECLARE(0x0228E520, s_network_arbitration_globals, network_arbitration_globals);

bool __cdecl network_arbitration_initialize()
{
	return INVOKE(0x004D9B30, network_arbitration_initialize);
}

