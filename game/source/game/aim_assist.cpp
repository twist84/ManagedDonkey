#include "game/aim_assist.hpp"

#include "main/global_preferences.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x0058A890, aim_assist_build_query_parameters);

//.text:0058A890 ; bool __cdecl aim_assist_build_query_parameters(int32, c_flags<e_aim_assist_query_flags, uint8, 2>, int16, s_aim_assist_query*)
bool __cdecl aim_assist_build_query_parameters(int32 unit_index, uint8 flags, int16 magnification_level, s_aim_assist_query* out_query)
{
	bool result = false;
	HOOK_INVOKE(result =, aim_assist_build_query_parameters, unit_index, flags, magnification_level, out_query);

	// if we aren't using a controller set the weapon magnetism angle to 0.0f
	if (input_abstraction_globals.controls_method != 1)
		out_query->weapon_magnetism_angle = 0.0f;

	return result;
}

//.text:0058AD40 ; bool __cdecl aim_assist_build_target(int32, real_point3d const*, real_vector3d const*, s_aim_assist_target*)
//.text:0058AF40 ; bool __cdecl aim_assist_build_target(s_aim_assist_query const*, int32, int32, bool, bool, bool, int32, s_model_target const*, real_matrix4x3 const*, real_matrix4x3 const*, real_point3d const*, real_vector3d const*, s_aim_assist_target*)
//.text:0058B5C0 ; 
//.text:0058B5D0 ; bool __cdecl aim_assist_clear_line_of_sight(real_point3d const*, real_point3d const*, real32, int32, int32, bool)
//.text:0058B6E0 ; bool __cdecl aim_assist_compute(int32, s_aim_assist_query const*, real_point3d const*, real_vector3d const*, int32, int32, c_game_team, s_aim_assist_target*)
//.text:0058B810 ; void __cdecl aim_assist_compute_target_velocity_offset(int32, real_point3d const*, real_point3d const*, real32, real32, real_vector3d*)
//.text:0058B930 ; int32 __cdecl aim_assist_compute_targets_from_object(s_aim_assist_query const*, int32, int32, bool, bool, real_point3d const*, real_vector3d const*, int32, c_static_array<s_aim_assist_target, 64>&)
//.text:0058BB80 ; 
//.text:0058BB90 ; bool __cdecl aim_assist_evaluate_target(s_aim_assist_query const*, real32, s_aim_assist_target*)
//.text:0058C070 ; bool __cdecl aim_assist_from_targeting(s_aim_assist_query const*, real_point3d const*, real_vector3d const*, int32, int32, c_game_team, s_aim_assist_targeting_result const*, bool, bool, s_aim_assist_target*)
//.text:0058C250 ; bool __cdecl aim_assist_get_ghost_reticule_position(int32, s_aim_assist_targeting_result const*, real_point3d*)
//.text:0058C460 ; void __cdecl aim_assist_get_implied_aiming_vector(int32, s_aim_assist_targeting_result const*, real_vector3d*)
//.text:0058C4D0 ; int32 __cdecl aim_assist_get_unit_weapon(int32)
//.text:0058C520 ; int32 __cdecl aim_assist_guess_weapon_projectile_definition_index(int32)
//.text:0058C5E0 ; bool __cdecl aim_assist_object_can_be_considered(int32)
//.text:0058C650 ; void __cdecl aim_assist_object_get_translational_velocity(int32, bool, real_vector3d*, int32*, int32*, int32*)
//.text:0058C8F0 ; bool __cdecl aim_assist_object_should_be_aimed_at(int32, int32, int32, c_game_team, bool, bool*, bool*)
//.text:0058CAA0 ; bool __cdecl aim_assist_object_within_cone(int32, real_point3d const*, real_vector3d const*, real32, real32, real32)
//.text:0058CB10 ; bool __cdecl aim_assist_retrieve_targeting_info(s_aim_assist_targeting_result const*, s_model_target const**, real_matrix4x3*, real_matrix4x3*, bool*)
//.text:0058CC50 ; void __cdecl aim_assist_targeting_clear(s_aim_assist_targeting_result*)
//.text:0058CC90 ; bool __cdecl aim_assist_targeting_decode(c_bitstream*, s_aim_assist_targeting_result*)
//.text:0058CD70 ; void __cdecl aim_assist_targeting_encode(c_bitstream*, s_aim_assist_targeting_result const*)
//.text:0058CF40 ; bool __cdecl aim_assist_targeting_valid(s_aim_assist_targeting_result const*)
//.text:0058CFC0 ; bool __cdecl aim_assist_teamless_unit_has_passenger(int32, c_game_team, bool*)
//.text:0058D0B0 ; bool __cdecl aim_assist_teamless_vehicle_should_be_aimed_at(int32, int32, int32, c_game_team, bool, bool*, bool*)
//.text:0058D1F0 ; 
//.text:0058D4E0 ; 
//.text:0058D4F0 ; 
//.text:0058D500 ; 
//.text:0058D510 ; int __cdecl compare_targets(void const*, void const*)
//.text:0058D5E0 ; 
//.text:0058D660 ; real64 __cdecl compute_composite_attenuation(real32, real32, real32, real32, real32, real32)
//.text:0058D790 ; 
//.text:0058D7A0 ; 
//.text:0058D7B0 ; 
//.text:0058D7C0 ; 
//.text:0058D7D0 ; 
//.text:0058D7E0 ; 
//.text:0058D7F0 ; int32 __cdecl find_aim_assist_targets(int32, s_aim_assist_query const*, s_cluster_reference, real_point3d const*, real_vector3d const*, int32, int32, c_game_team, c_static_array<s_aim_assist_target, 64>&)
//.text:0058D990 ; int32 __cdecl find_aim_assist_targets_recursive(s_aim_assist_query const*, int32, real_point3d const*, real_vector3d const*, real32, real32, real32, int32, int32, c_game_team, int32, c_static_array<s_aim_assist_target, 64>&)
//.text:0058DAE0 ; 
//.text:0058DAF0 ; 
//.text:0058DB00 ; 
//.text:0058DB10 ; 
//.text:0058DB20 ; 
//.text:0058DB30 ; 
//.text:0058DB40 ; 
//.text:0058DB50 ; 
//.text:0058DB60 ; 
//.text:0058DE40 ; bool __cdecl pin_aiming_for_player(int32)
//.text:0058DEA0 ; void __cdecl player_aim_assist(int32, uint8, real_vector3d const*, int16, s_aim_assist_control_result*, s_aim_assist_targeting_result*)
//.text:0058E390 ; int32 __cdecl player_aim_assist_get_player_under_crosshair(int32)
//.text:0058E7C0 ; void __cdecl player_aim_assist_update_lead_vector(int32, real_vector3d const*, s_aim_assist_targeting_result*)
//.text:0058EA50 ; bool __cdecl player_aim_projectile(int32, s_aim_assist_targeting_result const*, int32, e_projectile_source, bool, real_point3d const*, real_point3d const*, real_vector3d*)
//.text:0058EBD0 ; void __cdecl player_aim_projectile_build_auto_aim_cone_origin_and_projectile_direction(s_aim_assist_targeting_result const*, int32, bool, bool, bool, real32, real_vector3d const*, real_point3d*, real_vector3d*, real_point3d*)
//.text:0058ECD0 ; bool __cdecl player_aim_projectile_internal(int32, s_aim_assist_targeting_result const*, int32, bool, real_point3d const*, real_point3d const*, real_vector3d const*, bool, s_aim_projectile_internal_result*)
//.text:0058F1C0 ; void __cdecl player_build_auto_aim_cone_origin(int32, bool, real_vector3d const*, real_point3d*, real_point3d*)
//.text:0058F1F0 ; void __cdecl player_build_auto_aim_cone_origin_and_camera_direction(int32, bool, real_vector3d const*, real_point3d*, real_vector3d*, real_point3d*)
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
//.text:0058F590 ; int32 __cdecl unit_ignore_object_index_get(int32)
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

