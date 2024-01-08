#include "interface/chud/chud.hpp"

void __cdecl chud_submit_navpoint(long user_index, s_chud_navpoint* navpoint)
{
	INVOKE(0x00A8A9F0, chud_submit_navpoint, user_index, navpoint);
}

void __cdecl chud_update(real world_seconds_elapsed)
{
	INVOKE(0x00A8AAE0, chud_update, world_seconds_elapsed);
}

