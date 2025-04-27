#include "scenario/scenario_zone_sets.hpp"

#include "cache/cache_files.hpp"
#include "cutscene/cinematics_definitions.hpp"
#include "scenario/scenario_definitions.hpp"
#include "tag_files/tag_groups.hpp"

//.text:005FB160 ; bool __cdecl flags_are_subset_of(uint32, uint32)

uint32 __cdecl scenario_zone_set_bsp_active_mask_get(long zone_set_index)
{
	return INVOKE(0x005FB180, scenario_zone_set_bsp_active_mask_get, zone_set_index);
}

uint32 __cdecl scenario_zone_set_cinematic_zone_required_mask_get(long zone_set_index)
{
	return INVOKE(0x005FB1A0, scenario_zone_set_cinematic_zone_required_mask_get, zone_set_index);
}

uint32 __cdecl scenario_zone_set_compute_new_designer_zone_mask(uint32 a1, uint32 a2, uint32 a3)
{
	return INVOKE(0x005FB1C0, scenario_zone_set_compute_new_designer_zone_mask, a1, a2, a3);
}

uint32 __cdecl scenario_zone_set_designer_zone_forbidden_mask_get(long zone_set_index)
{
	return INVOKE(0x005FB210, scenario_zone_set_designer_zone_forbidden_mask_get, zone_set_index);
}

uint32 __cdecl scenario_zone_set_designer_zone_required_mask_get(long zone_set_index)
{
	return INVOKE(0x005FB240, scenario_zone_set_designer_zone_required_mask_get, zone_set_index);
}

//.text:005FB290 ; uint32 __cdecl scenario_zone_set_get_valid_designer_zones(long, uint32)
//.text:005FB2D0 ; bool __cdecl scenario_zone_set_switch_loads_anything(s_scenario_game_state const*, long)

char const* scenario_get_designer_zone_name(struct scenario* scenario, long designer_zone_index)
{
	return scenario->designer_zones[designer_zone_index].name.get_string();
}

char const* scenario_get_cinematic_zone_name(struct scenario* scenario, long cinematic_zone_index)
{
	long cinematic_index = scenario->cinematics[cinematic_zone_index].name.index;
	if (cinematic_index != NONE)
	{
		char const* cinematic_name = tag_get_name(cinematic_index);
		return tag_name_strip_path(cinematic_name);
	}
	return "";
}

