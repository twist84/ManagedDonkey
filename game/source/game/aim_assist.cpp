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
//.text:0058B810 ; void __cdecl aim_assist_compute_target_velocity_offset(long, real_point3d const*, real_point3d const*, real, real, vector3d*)
//.text:0058B930 ; long __cdecl aim_assist_compute_targets_from_object(s_aim_assist_query const*, long, long, bool, bool, real_point3d const*, vector3d const*, long, c_static_array<s_aim_assist_target, 64>&)
//.text:0058BB80 ; 
//.text:0058BB90 ; bool __cdecl aim_assist_evaluate_target(s_aim_assist_query const*, real, s_aim_assist_target*)
//.text:0058C070 ; bool __cdecl aim_assist_from_targeting(s_aim_assist_query const*, real_point3d const*, vector3d const*, long, long, c_game_team, s_aim_assist_targeting_result const*, bool, bool, s_aim_assist_target*)
//.text:0058C250 ; bool __cdecl aim_assist_get_ghost_reticule_position(long, s_aim_assist_targeting_result const*, real_point3d*)
//.text:0058C460 ; void __cdecl aim_assist_get_implied_aiming_vector(long, s_aim_assist_targeting_result const*, vector3d*)
//.text:0058C4D0 ; long __cdecl aim_assist_get_unit_weapon(long)
//.text:0058C520 ; long __cdecl aim_assist_guess_weapon_projectile_definition_index(long)
//.text:0058C5E0 ; bool __cdecl aim_assist_object_can_be_considered(long)
//.text:0058C650 ; void __cdecl aim_assist_object_get_translational_velocity(long, bool, vector3d*, long*, long*, long*)
//.text:0058C8F0 ; bool __cdecl aim_assist_object_should_be_aimed_at(long, long, long, c_game_team, bool, bool*, bool*)
//.text:0058CAA0 ; bool __cdecl aim_assist_object_within_cone(long, real_point3d const*, vector3d const*, real, real, real)
//.text:0058CB10 ; bool __cdecl aim_assist_retrieve_targeting_info(s_aim_assist_targeting_result const*, s_model_target const**, real_matrix4x3*, real_matrix4x3*, bool*)
//.text:0058CC50 ; void __cdecl aim_assist_targeting_clear(s_aim_assist_targeting_result*)
//.text:0058CC90 ; bool __cdecl aim_assist_targeting_decode(c_bitstream*, s_aim_assist_targeting_result*)
//.text:0058CD70 ; void __cdecl aim_assist_targeting_encode(c_bitstream*, s_aim_assist_targeting_result const*)
//.text:0058CF40 ; bool __cdecl aim_assist_targeting_valid(s_aim_assist_targeting_result const*)
//.text:0058CFC0 ; bool __cdecl aim_assist_teamless_unit_has_passenger(long, c_game_team, bool*)
//.text:0058D0B0 ; bool __cdecl aim_assist_teamless_vehicle_should_be_aimed_at(long, long, long, c_game_team, bool, bool*, bool*)
//.text:0058D1F0 ; 
//.text:0058D4E0 ; 
//.text:0058D4F0 ; 
//.text:0058D500 ; 
//.text:0058D510 ; int __cdecl compare_targets(void const*, void const*)
//.text:0058D5E0 ; 
//.text:0058D660 ; double __cdecl compute_composite_attenuation(real, real, real, real, real, real)
//.text:0058D790 ; 
//.text:0058D7A0 ; 
//.text:0058D7B0 ; 
//.text:0058D7C0 ; 
//.text:0058D7D0 ; 
//.text:0058D7E0 ; 
//.text:0058D7F0 ; long __cdecl find_aim_assist_targets(long, s_aim_assist_query const*, s_cluster_reference, real_point3d const*, vector3d const*, long, long, c_game_team, c_static_array<s_aim_assist_target, 64>&)
//.text:0058D990 ; long __cdecl find_aim_assist_targets_recursive(s_aim_assist_query const*, long, real_point3d const*, vector3d const*, real, real, real, long, long, c_game_team, long, c_static_array<s_aim_assist_target, 64>&)
//.text:0058DAE0 ; 
//.text:0058DAF0 ; 
//.text:0058DB00 ; 
//.text:0058DB10 ; 
//.text:0058DB20 ; 
//.text:0058DB30 ; 
//.text:0058DB40 ; 
//.text:0058DB50 ; 
//.text:0058DB60 ; 
//.text:0058DE40 ; bool __cdecl pin_aiming_for_player(long)
//.text:0058DEA0 ; void __cdecl player_aim_assist(e_input_user_index, byte, vector3d const*, short, s_aim_assist_control_result*, s_aim_assist_targeting_result*)
//.text:0058E390 ; long __cdecl player_aim_assist_get_player_under_crosshair(long)
//.text:0058E7C0 ; void __cdecl player_aim_assist_update_lead_vector(long, vector3d const*, s_aim_assist_targeting_result*)
//.text:0058EA50 ; bool __cdecl player_aim_projectile(long, s_aim_assist_targeting_result const*, long, e_projectile_source, bool, real_point3d const*, real_point3d const*, vector3d*)
//.text:0058EBD0 ; void __cdecl player_aim_projectile_build_auto_aim_cone_origin_and_projectile_direction(s_aim_assist_targeting_result const*, long, bool, bool, bool, real, vector3d const*, real_point3d*, vector3d*, real_point3d*)
//.text:0058ECD0 ; bool __cdecl player_aim_projectile_internal(long, s_aim_assist_targeting_result const*, long, bool, real_point3d const*, real_point3d const*, vector3d const*, bool, s_aim_projectile_internal_result*)
//.text:0058F1C0 ; void __cdecl player_build_auto_aim_cone_origin(long, bool, vector3d const*, real_point3d*, real_point3d*)
//.text:0058F1F0 ; void __cdecl player_build_auto_aim_cone_origin_and_camera_direction(long, bool, vector3d const*, real_point3d*, vector3d*, real_point3d*)
//.text:0058F330 ; 
//.text:0058F350 ; 
//.text:0058F370 ; 
//.text:0058F380 ; 
//.text:0058F4D0 ; 
//.text:0058F4F0 ; 
//.text:0058F510 ; 
//.text:0058F530 ; 
//.text:0058F550 ; 
//.text:0058F570 ; 
//.text:0058F590 ; long __cdecl unit_ignore_object_index_get(long)
//.text:0058F660 ; 
//.text:0058F680 ; 
//.text:0058F690 ; 
//.text:0058F6A0 ; 
//.text:0058F6B0 ; 
//.text:0058F6C0 ; 
//.text:0058F6E0 ; 

void aim_assist_debug_render()
{
}

