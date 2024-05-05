#include "effects/effects.hpp"

bool debug_damage_effects = false;

void render_debug_damage_effects()
{
	if (debug_damage_effects)
	{

	}
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

