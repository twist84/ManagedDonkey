#include "interface/user_interface_network_search.hpp"

#include "interface/user_interface_memory.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_search.hpp"

REFERENCE_DECLARE(0x05270A90, s_user_interface_network_search_globals, user_interface_network_search_globals);

HOOK_DECLARE(0x00AD4380, user_interface_available_squads_initialize);

bool __cdecl user_interface_available_squads_initialize(int32 controller_index, uns32 available_squad_search_flags, char category)
{
	c_allocation_base* allocation = user_interface_memory_allocation();
	user_interface_network_search_globals.network_search_valid = network_search_start(controller_index, category != 0, 4 /* default is `25`, this causes an issue not having enough memory allocate */, available_squad_search_flags, allocation);

	return user_interface_network_search_globals.network_search_valid;
}

