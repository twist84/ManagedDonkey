#include "units/giants.hpp"

REFERENCE_DECLARE(0x0194AA18, int32, g_giant_hunt_player);                     // unreferenced
REFERENCE_DECLARE(0x0194AA1C, real32, g_giant_hunting_min_radius);             // unreferenced
REFERENCE_DECLARE(0x0194AA20, real32, g_giant_hunting_max_radius);             // unreferenced
REFERENCE_DECLARE(0x0194AA24, real32, g_giant_hunting_throttle_scale);         // unreferenced
REFERENCE_DECLARE(0x0194AA28, int32, g_giant_weapon_wait_time);                // unreferenced
REFERENCE_DECLARE(0x0194AA2C, int32, g_giant_weapon_trigger_time);             // unreferenced
REFERENCE_DECLARE(0x0194AA30, bool, g_giant_foot_ik);                          // referenced
REFERENCE_DECLARE(0x0194AA31, bool, g_giant_ankle_ik);                         // referenced
REFERENCE_DECLARE(0x0194AA32, bool, g_giant_elevation_control);                // unreferenced
REFERENCE_DECLARE(0x0194AA33, bool, g_giant_buckle_rotation);                  // referenced
REFERENCE_DECLARE(0x0194AA34, real32, g_giant_custom_animation_recovery_time); // referenced

//.text:00BBB480 ; real64 __cdecl compute_score_of_buckle_angles(real32, real32, const real_matrix4x3*, real_matrix4x3*)
//.text:00BBB650 ; public: const c_model_animation* __cdecl c_animation_manager::get_active_animation() const
//.text:00BBB650 ; 
//.text:00BBB660 ; 
//.text:00BBB680 ; 
//.text:00BBB6A0 ; void __cdecl giant_adjust_node_matrices(int32, int32, real_matrix4x3*)
//.text:00BBBAE0 ; void __cdecl giant_animation_setup_mixing_board(c_animation_manager*)

real32 __cdecl giant_buckling_magnitude_get(int32 giant_index)
{
	return INVOKE(0x00BBBB20, giant_buckling_magnitude_get, giant_index);
}

//.text:00BBBBD0 ; void __cdecl giant_clear_foot_target_flags(int32)
//.text:00BBBC20 ; void __cdecl giant_clear_foot_tracking_data(int32)
//.text:00BBBED0 ; bool __cdecl giant_compute_function_value(int32, int32, int32, real32*, bool*, bool*)
//.text:00BBBF70 ; void __cdecl giant_delete(int32)
//.text:00BBBF80 ; bool __cdecl giant_find_ground_point(int32, const real_point3d*, real_point3d*, real_vector3d*, real_point3d*)
//.text:00BBC100 ; void __cdecl giant_find_pathfinding_location(int32, int16*, int32*, c_sector_ref*, real_point3d*, int32*, uns32*)
//.text:00BBC350 ; const giant_buckle_parameters* __cdecl giant_get_current_buckle_parameters(int32)
//.text:00BBC3A0 ; int32 __cdecl giant_get_current_buckle_state_name(int32)
//.text:00BBC400 ; bool __cdecl giant_handle_motor_update(int32, bool)
//.text:00BBC6A0 ; bool __cdecl giant_ik_foot_chain(int32, real_point3d*, real32, int32, real32, real32, int32, real_matrix4x3*)
//.text:00BBCDF0 ; void __cdecl giant_locate_auxilary_animations(int32)
//.text:00BBCE80 ; bool __cdecl giant_new(int32, object_placement_data*, bool*)
//.text:00BBCF40 ; int16 __cdecl giant_pathfinding_policy(int32, const s_scenario_giant*)
//.text:00BBCF80 ; void __cdecl giant_place(int32, s_scenario_giant*)
//.text:00BBD020 ; bool __cdecl giant_predict_future_foot_position(int32, int32, int32, real_point3d*, real_point3d*, real_vector3d*, real_point3d*)
//.text:00BBD080 ; void __cdecl giant_preprocess_node_orientations(int32, const c_static_flags<255>*, int32, real_orientation*)
//.text:00BBD170 ; void __cdecl giant_render_debug(int32)
//.text:00BBD180 ; void __cdecl giant_set_buckle_desire(int32, bool)
//.text:00BBD340 ; void __cdecl giant_set_crouch_desire(int32, bool)
//.text:00BBD390 ; void __cdecl giant_set_death_desire(int32, bool)
//.text:00BBD400 ; void __cdecl giant_setup_buckling_parameters(int32)
//.text:00BBD990 ; void __cdecl giant_setup_foot_tracking_data(int32)
//.text:00BBDA20 ; bool __cdecl giant_solve_future_marker_position(int32, int32, int32, real_matrix4x3*)
//.text:00BBDD90 ; bool __cdecl giant_update(int32)
//.text:00BBDEE0 ; void __cdecl giant_update_crouch(int32)
//.text:00BBDF60 ; void __cdecl giant_update_elevation(int32, real32)
//.text:00BBE070 ; void __cdecl giant_update_foot_tracking(int32)
//.text:00BBE1A0 ; bool __cdecl giant_waiting_to_buckle(int32)
//.text:00BBE1D0 ; bool __cdecl giant_waiting_to_buckle_or_buckling(int32)
//.text:00BBE250 ; 
//.text:00BBE2B0 ; 
//.text:00BBE2E0 ; 
//.text:00BBE300 ; 
//.text:00BBE320 ; void __cdecl setup_buckling_globals(int32, int32, int32, const real_matrix4x3*, real32, real32, real32, real32)
//.text:00BBE590 ; 
//.text:00BBE5B0 ; 

