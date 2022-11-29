#include "interface/chud/chud.hpp"

void __cdecl chud_submit_navpoint(long user_index, s_chud_navpoint* navpoint)
{
	INVOKE(0x00A8A9F0, chud_submit_navpoint, user_index, navpoint);
}

