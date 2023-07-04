#include "physics/havok.hpp"

#include "cseries/cseries.hpp"

void __cdecl havok_can_modify_state_allow()
{
	INVOKE(0x005C45C0, havok_can_modify_state_allow);
}

void __cdecl havok_can_modify_state_disallow()
{
	INVOKE(0x005C45D0, havok_can_modify_state_disallow);
}

