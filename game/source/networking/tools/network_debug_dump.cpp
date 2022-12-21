#include "networking/tools/network_debug_dump.hpp"

REFERENCE_DECLARE(0x0199C0B0, s_netdebug_globals, g_netdebug_globals);
REFERENCE_DECLARE(0x0199FA20, c_synchronized_long, g_net_debug_thread_has_exited);

void __cdecl get_system_ip_addresses(c_static_string<16>* insecure_ip, c_static_string<128>* secure_ip)
{
	return INVOKE(0x0042FB90, get_system_ip_addresses, insecure_ip, secure_ip);
}

