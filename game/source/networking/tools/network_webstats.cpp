#include "networking/tools/network_webstats.hpp"

void __cdecl network_webstats_force_reset()
{
	INVOKE(0x004E4020, network_webstats_force_reset);
}

void __cdecl network_webstats_update()
{
	INVOKE(0x004E4320, network_webstats_update);
}

