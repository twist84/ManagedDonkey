#include "game/aim_assist.hpp"

#include "main/global_preferences.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x0058A890, aim_assist_build_query_parameters);

//.text:0058A890 ; bool __cdecl aim_assist_build_query_parameters(long, c_flags<e_aim_assist_query_flags, byte, 2>, short, s_aim_assist_query*)
bool __cdecl aim_assist_build_query_parameters(long unit_index, byte flags, short magnification_level, s_aim_assist_query* out_query)
{
	bool result = false;
	HOOK_INVOKE(result =, aim_assist_build_query_parameters, unit_index, flags, magnification_level, out_query);

	// if we aren't using a controller set the weapon magnetism angle to 0.0f
	if (input_abstraction_globals.controls_method != 1)
		out_query->weapon_magnetism_angle = 0.0f;

	return result;
}

//.text:0058AD40 ; bool __cdecl aim_assist_build_target(long, real_point3d const*, vector3d const*, s_aim_assist_target*)
//.text:0058AF40 ; bool __cdecl aim_assist_build_target(s_aim_assist_query const*, long, long, bool, bool, bool, long, s_model_target const*, real_matrix4x3 const*, real_matrix4x3 const*, real_point3d const*, vector3d const*, s_aim_assist_target*)
//.text:0058B5C0 ; 
//.text:0058B5D0 ; bool __cdecl aim_assist_clear_line_of_sight(real_point3d const*, real_point3d const*, real, long, long, bool)
//.text:0058B6E0 ; bool __cdecl aim_assist_compute(long, s_aim_assist_query const*, real_point3d const*, vector3d const*, long, long, c_game_team, s_aim_assist_target*)

void aim_assist_debug_render()
{
}

