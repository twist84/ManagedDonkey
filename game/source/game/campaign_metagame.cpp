#include "game/campaign_metagame.hpp"

REFERENCE_DECLARE(0x0244A898, bool, chud_debug_metagame);
REFERENCE_DECLARE(0x0244A8A0, s_campaign_metagame_runtime_globals_secondary, g_campaign_metagame_runtime_globals_secondary);
REFERENCE_DECLARE(0x0244C9B8, dword, dword_244C9B8);

void __cdecl campaign_metagame_update()
{
	INVOKE(0x0060A4F0, campaign_metagame_update);
}

bool __cdecl sub_60B080()
{
	return INVOKE(0x0060B080, sub_60B080);
}

