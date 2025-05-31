#include "ai/path.hpp"

//.text:014BC190 ; int16 __cdecl build_path_links_for_sector(const path_node*, path_link*, const path_state*)
//.text:014BCA50 ; int16 __cdecl build_raw_path(int32, path_state*, path_result*, int16, int16, path_step*, bool*, int16*, path_step_source*, int16)
//.text:014BD0D0 ; 
//.text:014BD0E0 ; void __cdecl closest_point_to_attractor(const real_point3d*, const real_point3d*, const real_point3d*, real_point3d*)
//.text:014BD1D0 ; real32 __cdecl path_attractor_weight(const path_state*, const c_ai_point3d*, const c_ai_point3d*, real32*)
//.text:014BD380 ; bool __cdecl path_error_info_contains_link(const path_error_info*, int32)
//.text:014BD3E0 ; void __cdecl path_error_stage_advance(path_error_info*)
//.text:014BD400 ; void __cdecl path_heap_bubble_down(path_state*, int16)
//.text:014BD500 ; void __cdecl path_heap_bubble_up(path_state*, int16)
//.text:014BD5B0 ; void __cdecl path_heap_insert(path_state*, int16, int16)
//.text:014BD600 ; int16 __cdecl path_heap_pop_cheapest_node(path_state*)
//.text:014BD670 ; void __cdecl path_input_new(path_input*, real32, bool, int32, real32)
//.text:014BD6C0 ; void __cdecl path_input_set_attractor(path_input*, const real_point3d*, real32, real32, int32, real32, bool)
//.text:014BD710 ; void __cdecl path_input_set_search_bounds(path_input*, real32)
//.text:014BD730 ; void __cdecl path_input_set_start(path_input*, const c_ai_point3d*, c_sector_ref)
//.text:014BD760 ; void __cdecl path_input_set_target_object(path_input*, int32)
//.text:014BD770 ; int16 __cdecl path_node_from_hash_table(const path_state*, c_sector_ref, int32)
//.text:014BD7F0 ; bool __cdecl path_state_approach_point(path_state*, const c_ai_point3d*, c_sector_ref, bool*, c_ai_point3d*)
//.text:014BD8F0 ; bool __cdecl path_state_begin(path_state*)
//.text:014BDBD0 ; bool __cdecl path_state_build_path(int32, path_state*, path_result*)
//.text:014BE080 ; void __cdecl path_state_destination(path_state*, const c_ai_point3d*, c_sector_ref, real32)
//.text:014BE0F0 ; void __cdecl path_state_destination(path_state*, c_path_destination*)
//.text:014BE140 ; bool __cdecl path_state_estimated_distance(path_state*, const c_ai_point3d*, c_sector_ref, real32*, real32*, real_vector3d*)

bool __cdecl path_state_find(path_state* state)
{
	return INVOKE(0x014BE6D0, path_state_find, state);
}

//.text:014BE780 ; void __cdecl path_state_new(const path_input*, path_state*, const path_error_info*, path_debug_storage*, const special_movement*, const s_hint_penalty_cache*)
//.text:014BE8F0 ; void __cdecl path_state_reset(path_state*)
//.text:014BE950 ; bool __cdecl path_state_traverse(path_state*)
//.text:014BF7A0 ; real32 __cdecl path_state_unit_distance(path_state*, const c_ai_point3d*, c_sector_ref, int32)
//.text:014BF8F0 ; bool __cdecl path_thresholds_equal(const path_step_source*, const path_step_source*)

void __cdecl paths_dispose()
{
	INVOKE(0x014BF920, paths_dispose);
}

void __cdecl paths_dispose_from_old_map()
{
	INVOKE(0x014BF930, paths_dispose_from_old_map);
}

void __cdecl paths_initialize()
{
	INVOKE(0x014BF940, paths_initialize);
}

void __cdecl paths_initialize_for_new_map()
{
	INVOKE(0x014BF950, paths_initialize_for_new_map);
}

//.text:014BF960 ; bool __cdecl process_step(const path_step*)
//.text:014BF990 ; public: void __cdecl c_path_destination::set(const c_ai_point3d*, const c_sector_ref*, real32)

