#include "motor/motor_system_biped.hpp"

//.text:00BAD3B0 ; void __cdecl biped_adjust_aiming(long, euler_angles2d*, real*, real*)
//.text:00BADC30 ; void __cdecl biped_build_axes_from_desired_facing(vector3d const*, vector3d const*, vector3d const*, vector3d*)
//.text:00BAE000 ; double __cdecl biped_calculate_crouch_delta(long)
//.text:00BAE130 ; bool __cdecl biped_calculate_crouch_velocity_delta(long, real*)
//.text:00BAE1F0 ; bool __cdecl biped_can_melee_target_unit(long, long)
//.text:00BAE380 ; void __cdecl biped_clear_pathfinding_cache(long)
//.text:00BAE400 ; void __cdecl biped_falling_damage(long)
//.text:00BAE600 ; void __cdecl biped_get_control_vectors(long, bool, vector3d*, vector3d*, vector3d*)
//.text:00BAE8D0 ; double __cdecl biped_get_player_specific_scale(long)
//.text:00BAE9D0 ; void __cdecl biped_ground_plane_fix_transform_from_physics(long, real_matrix4x3*)
//.text:00BAED40 ; void __cdecl biped_ground_plane_fix_transform_to_physics(long, real_matrix4x3*)
//.text:00BAF0C0 ; void __cdecl biped_initialize_character_physics_move_input(long, s_character_physics_move_input_datum*, bool, bool, bool, bool, bool)
//.text:00BAF280 ; void __cdecl biped_initialize_character_physics_update_input(long, s_character_physics_update_input_datum*, bool, bool, real, bool, bool)
//.text:00BAF990 ; void __cdecl biped_move_calculate_controlled_by_aiming(long, vector3d const*, vector3d const*, vector3d const*, bool, vector3d*, short*)
//.text:00BB01B0 ; void __cdecl biped_physics_update_crouch(long, s_character_physics_update_output_datum*, bool*)
//.text:00BB0790 ; void __cdecl biped_set_forward_vector(long, vector3d const*)
//.text:00BB08A0 ; void __cdecl biped_snap_facing_to_desired_facing(long)
//.text:00BB08E0 ; void __cdecl biped_update_desired_facing(long, bool, short*, vector3d*, bool*)
//.text:00BB0B90 ; void __cdecl biped_update_pathfinding_cache(long, s_character_physics_move_output_datum const*)
//.text:00BB0D60 ; 
//.text:00BB0D70 ; public: class c_animation_id __cdecl c_animation_manager::get_active_animation_id() const
//.text:00BB0D90 ; 
//.text:00BB0E10 ; bool __cdecl motor_system_biped_move(long, bool, bool, bool, bool, s_character_physics_move_output_datum*)
//.text:00BB1890 ; void __cdecl motor_system_biped_physics_postupdate_moving(long, s_character_physics_update_input_datum const*, s_character_physics_update_output_datum*, bool*)
//.text:00BB1A50 ; bool __cdecl motor_system_biped_physics_preupdate_moving(long, s_character_physics_update_input_datum*, s_character_physics_update_output_datum*, bool, bool, real, bool, bool)

