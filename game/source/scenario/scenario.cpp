#include "scenario/scenario.hpp"

#include "cache/cache_files.hpp"
#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "hf2p/hf2p.hpp"
#include "main/levels.hpp"
#include "memory/module.hpp"
#include "scenario/scenario_definitions.hpp"
#include "scenario/scenario_tags_fixup.hpp"
#include "tag_files/tag_groups.hpp"

#include <assert.h>

HOOK_DECLARE(0x004EA5E0, scenario_load);

long& global_scenario_index = *reinterpret_cast<long*>(0x0189CCF8);

scenario*& global_scenario = *reinterpret_cast<scenario**>(0x022AAEB4);

scenario* global_scenario_get()
{
	FUNCTION_BEGIN(true);

	// halo online
	return tag_get<scenario>('scnr', global_scenario_index);

	// halo 3
	//assert(global_scenario);
	//return global_scenario;
}

//bool scenario_tags_match(enum e_campaign_id, enum e_map_id, char const*)
bool __cdecl scenario_tags_match(long campaign_id, long map_id, char const* scenario_path)
{
	FUNCTION_BEGIN(true);

	//return INVOKE(0x004EB820, scenario_tags_match, campaign_id, map_id, scenario_path);

	assert(scenario_path != 0);

	scenario* global_scenario = global_scenario_get();
	if (levels_map_id_is_fake(map_id) || (campaign_id == -1 && map_id == -1))
		return true;

	if (global_scenario->campaign_id == campaign_id && global_scenario->map_id == map_id)
		return true;

	return false;
}

void __cdecl scenario_invalidate()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x004EA3E0, scenario_invalidate);
}

//bool __cdecl scenario_load(enum e_campaign_id, enum e_map_id, char const*)
bool __cdecl scenario_load(long campaign_id, long map_id, char const* scenario_path)
{
	FUNCTION_BEGIN(true);

	//bool result = false;
	//HOOK_INVOKE(result =, scenario_load, campaign_id, map_id, scenario_path);
	//return result;

	// saber function, crashes in `hf2p_game_client_cache_release.exe!sub_A28EC0` if not called
	hf2p_scenario_load();

	if (scenario_tags_load(scenario_path))
	{
		if (scenario_tags_match(campaign_id, map_id, scenario_path))
		{
			scenario_tags_fixup();
			scenario_tags_load_finished();
			hf2p_scenario_tags_load_finished();

			return true;
		}
		else
		{
			scenario_tags_unload();
			scenario_invalidate();

			return false;
		}
	}

	tag_load_missing_tags_report();
	return false;
}