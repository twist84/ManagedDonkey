#include "interface/user_interface_network_search.hpp"

#include "interface/user_interface_memory.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_search.hpp"

HOOK_DECLARE(0x00AD4380, user_interface_available_squads_initialize);

REFERENCE_DECLARE(0x05270A90, bool, g_available_squads_ready);

bool __cdecl user_interface_available_squads_initialize(long controller_index, dword_flags available_squad_search_flags, char category)
{
	c_allocation_base* allocation = user_interface_memory_allocation();
	g_available_squads_ready = network_search_start(controller_index, category != 0, 4 /* default is `25`, this causes an issue not having enough memory allocate */, available_squad_search_flags, allocation);

	return g_available_squads_ready;
}

