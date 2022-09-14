#include "scenario/scenario.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "main/levels.hpp"
#include "scenario/scenario_definitions.hpp"
#include "scenario/scenario_tags_fixup.hpp"
#include "tag_files/tag_groups.hpp"

#include <assert.h>

long& global_scenario_index = *reinterpret_cast<long*>(0x0189CCF8);

scenario*& global_scenario = *reinterpret_cast<scenario**>(0x022AAEB4);

scenario* global_scenario_get()
{
	// halo online
	return tag_get<scenario>('scnr', global_scenario_index);

	// halo 3
	//assert(global_scenario);
	//return global_scenario;
}

//bool scenario_tags_match(enum e_campaign_id, enum e_map_id, char const*)
bool __cdecl scenario_tags_match(long campaign_id, long map_id, char const* scenario_path)
{
	//return DECLTHUNK(0x004EB820, scenario_tags_match, campaign_id, map_id, scenario_path);

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
	return DECLTHUNK(0x004EA3E0, scenario_invalidate);
}

//bool __cdecl scenario_load(enum e_campaign_id, enum e_map_id, char const*)
bool __cdecl scenario_load(long campaign_id, long map_id, char const* scenario_path)
{
    //return DECLTHUNK(0x004EA5E0, scenario_load, campaign_id, map_id, scenario_path);

	// saber function, crashes in `hf2p_game_client_cache_release.exe!sub_A28EC0` if not called
	DECLFUNC(0x00600770, void, __cdecl)();

	if (scenario_tags_load(scenario_path))
	{
		if (scenario_tags_match(campaign_id, map_id, scenario_path))
		{
			scenario_tags_fixup();
			scenario_tags_load_finished();

			// saber function, uses `g_hf2p_first_run`
			//game_perform_startup();

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