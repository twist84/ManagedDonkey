#include "networking/tools/network_debug_dump.hpp"

s_netdebug_globals& g_netdebug_globals = *reinterpret_cast<s_netdebug_globals*>(0x0199C0B0);
c_synchronized_long& g_net_debug_thread_has_exited = *reinterpret_cast<c_synchronized_long*>(0x0199FA20);

