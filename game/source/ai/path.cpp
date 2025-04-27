#include "ai/path.hpp"

//.text:014BC190 ; short __cdecl build_path_links_for_sector(path_node const*, path_link*, path_state const*)
//.text:014BCA50 ; short __cdecl build_raw_path(long, path_state*, path_result*, short, short, path_step*, bool*, short*, path_step_source*, short)
//.text:014BD0D0 ; 
//.text:014BD0E0 ; void __cdecl closest_point_to_attractor(real_point3d const*, real_point3d const*, real_point3d const*, real_point3d*)
//.text:014BD1D0 ; real32 __cdecl path_attractor_weight(path_state const*, c_ai_point3d const*, c_ai_point3d const*, real32*)
//.text:014BD380 ; bool __cdecl path_error_info_contains_link(path_error_info const*, long)
//.text:014BD3E0 ; void __cdecl path_error_stage_advance(path_error_info*)
//.text:014BD400 ; void __cdecl path_heap_bubble_down(path_state*, short)
//.text:014BD500 ; void __cdecl path_heap_bubble_up(path_state*, short)
//.text:014BD5B0 ; void __cdecl path_heap_insert(path_state*, short, short)
//.text:014BD600 ; short __cdecl path_heap_pop_cheapest_node(path_state*)
//.text:014BD670 ; void __cdecl path_input_new(path_input*, real32, bool, long, real32)
//.text:014BD6C0 ; void __cdecl path_input_set_attractor(path_input*, real_point3d const*, real32, real32, long, real32, bool)
//.text:014BD710 ; void __cdecl path_input_set_search_bounds(path_input*, real32)
//.text:014BD730 ; void __cdecl path_input_set_start(path_input*, c_ai_point3d const*, c_sector_ref)
//.text:014BD760 ; void __cdecl path_input_set_target_object(path_input*, long)
//.text:014BD770 ; short __cdecl path_node_from_hash_table(path_state const*, c_sector_ref, long)
//.text:014BD7F0 ; bool __cdecl path_state_approach_point(path_state*, c_ai_point3d const*, c_sector_ref, bool*, c_ai_point3d*)
//.text:014BD8F0 ; bool __cdecl path_state_begin(path_state*)
//.text:014BDBD0 ; bool __cdecl path_state_build_path(long, path_state*, path_result*)
//.text:014BE080 ; void __cdecl path_state_destination(path_state*, c_ai_point3d const*, c_sector_ref, real32)
//.text:014BE0F0 ; void __cdecl path_state_destination(path_state*, c_path_destination*)
//.text:014BE140 ; bool __cdecl path_state_estimated_distance(path_state*, c_ai_point3d const*, c_sector_ref, real32*, real32*, real_vector3d*)

bool __cdecl path_state_find(path_state* state)
{
	return INVOKE(0x014BE6D0, path_state_find, state);
}

//.text:014BE780 ; void __cdecl path_state_new(path_input const*, path_state*, path_error_info const*, path_debug_storage*, special_movement const*, s_hint_penalty_cache const*)
//.text:014BE8F0 ; void __cdecl path_state_reset(path_state*)
//.text:014BE950 ; bool __cdecl path_state_traverse(path_state*)
//.text:014BF7A0 ; real32 __cdecl path_state_unit_distance(path_state*, c_ai_point3d const*, c_sector_ref, long)
//.text:014BF8F0 ; bool __cdecl path_thresholds_equal(path_step_source const*, path_step_source const*)

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

//.text:014BF960 ; bool __cdecl process_step(path_step const*)
//.text:014BF990 ; public: void __cdecl c_path_destination::set(c_ai_point3d const*, c_sector_ref const*, real32)

