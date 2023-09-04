#include "physics/havok.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x04B1DA80, bool, g_havok_memory_always_system);

void __cdecl havok_can_modify_state_allow()
{
	INVOKE(0x005C45C0, havok_can_modify_state_allow);
}

void __cdecl havok_can_modify_state_disallow()
{
	INVOKE(0x005C45D0, havok_can_modify_state_disallow);
}

