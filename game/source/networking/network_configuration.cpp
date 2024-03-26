#include "networking/network_configuration.hpp"

REFERENCE_DECLARE(0x019A62B8, bool, g_network_configuration_initialized);
REFERENCE_DECLARE(0x019A62C0, s_network_configuration, g_network_configuration);

void __cdecl network_configuration_initialize(bool a1)
{
	INVOKE(0x0043C960, network_configuration_initialize, a1);

	//network_configuration_initialize_private();
	//g_network_configuration_download_buffer.__unknown684 = a1 ? 0 : 2;
	//g_network_configuration_initialized = true;
}

