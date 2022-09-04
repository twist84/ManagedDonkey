#include "networking/network_memory.hpp"

s_network_shared_memory_globals& network_shared_memory_globals = *reinterpret_cast<s_network_shared_memory_globals*>(0x01A28D58);
s_network_base_memory_globals& network_base_memory_globals = *reinterpret_cast<s_network_base_memory_globals*>(0x01A28D88);