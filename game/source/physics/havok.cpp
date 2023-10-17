#include "physics/havok.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x018A2324, s_havok_constants, g_havok_constants);
REFERENCE_DECLARE(0x04B1DA80, bool, g_havok_memory_always_system);

void __cdecl havok_can_modify_state_allow()
{
	INVOKE(0x005C45C0, havok_can_modify_state_allow);
}

void __cdecl havok_can_modify_state_disallow()
{
	INVOKE(0x005C45D0, havok_can_modify_state_disallow);
}

void havok_debug_render()
{
	// #TODO: implement
}

