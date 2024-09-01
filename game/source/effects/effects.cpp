#include "effects/effects.hpp"

bool debug_damage_effects = false;

void __cdecl effects_prepare_for_new_zone_set(dword a1, dword a2)
{
	INVOKE(0x00530200, effects_prepare_for_new_zone_set, a1, a2);
}

void __cdecl effects_initialize_for_new_non_bsp_zone_set(s_game_non_bsp_zone_set const* new_non_bsp_zone_set)
{
	INVOKE(0x00530210, effects_initialize_for_new_non_bsp_zone_set, new_non_bsp_zone_set);
}

void __cdecl effects_dispose_from_old_non_bsp_zone_set(s_game_non_bsp_zone_set const* old_non_bsp_zone_set)
{
	INVOKE(0x00530220, effects_dispose_from_old_non_bsp_zone_set, old_non_bsp_zone_set);
}

void __cdecl effects_dispose()
{
	INVOKE(0x005BBE70, effects_dispose);
}

void __cdecl effects_dispose_from_old_map()
{
	INVOKE(0x005BBF10, effects_dispose_from_old_map);
}

void __cdecl effects_dispose_from_old_structure_bsp(dword old_structure_bsp_mask)
{
	INVOKE(0x005BBFD0, effects_dispose_from_old_structure_bsp, old_structure_bsp_mask);
}

void __cdecl effects_initialize()
{
	INVOKE(0x005BC740, effects_initialize);
}

void __cdecl effects_initialize_for_new_map()
{
	INVOKE(0x005BC990, effects_initialize_for_new_map);
}

void __cdecl effects_initialize_for_new_structure_bsp(dword new_structure_bsp_mask)
{
	INVOKE(0x005BCA80, effects_initialize_for_new_structure_bsp, new_structure_bsp_mask);
}

void __cdecl effects_prepare_for_non_bsp_zone_set_switch(s_game_non_bsp_zone_set const* old_non_bsp_zone_set, s_game_non_bsp_zone_set const* new_non_bsp_zone_set, c_scenario_resource_registry* resource_registry)
{
	INVOKE(0x005BCE50, effects_prepare_for_non_bsp_zone_set_switch, old_non_bsp_zone_set, new_non_bsp_zone_set, resource_registry);
}

bool __cdecl dangerous_effects_near_player()
{
    return INVOKE(0x005B4DA0, dangerous_effects_near_player);
}

//long __cdecl effect_new_from_point_vector(long, real_point3d const*, vector3d const*, vector3d const*, e_match_all_markers, e_effect_deterministic, plane3d const*, s_cluster_reference*)
long __cdecl effect_new_from_point_vector(long effect_index, real_point3d const* position, vector3d const* forward, vector3d const* normal, long match_all_markers, long effect_deterministic, plane3d const* plane, s_cluster_reference* cluster_reference)
{
	return INVOKE(0x005B87F0, effect_new_from_point_vector, effect_index, position, forward, normal, match_all_markers, effect_deterministic, plane, cluster_reference);
}

void __cdecl effects_update()
{
	INVOKE(0x005BD680, effects_update);
}

void render_debug_damage_effects()
{
	if (debug_damage_effects)
	{

	}
}

