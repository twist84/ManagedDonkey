#include "game/campaign_metagame.hpp"

REFERENCE_DECLARE(0x0244A898, bool, chud_debug_metagame);
REFERENCE_DECLARE(0x0244A8A0, s_campaign_metagame_runtime_globals_secondary, g_campaign_metagame_runtime_globals_secondary);
REFERENCE_DECLARE(0x0244C9B8, dword, dword_244C9B8);

void __cdecl campaign_metagame_dispose()
{
	INVOKE(0x00606F90, campaign_metagame_dispose);
}

void __cdecl campaign_metagame_dispose_from_old_map()
{
	INVOKE(0x00606FB0, campaign_metagame_dispose_from_old_map);
}

void __cdecl campaign_metagame_initialize()
{
	INVOKE(0x00607C80, campaign_metagame_initialize);
}

void __cdecl campaign_metagame_initialize_for_new_map()
{
	INVOKE(0x00607CE0, campaign_metagame_initialize_for_new_map);
}

void __cdecl campaign_metagame_update()
{
	INVOKE(0x0060A4F0, campaign_metagame_update);
}

bool __cdecl sub_60B080()
{
	return INVOKE(0x0060B080, sub_60B080);
}

