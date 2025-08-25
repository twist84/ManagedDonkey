#include "items/projectiles.hpp"

bool debug_projectiles = false;

bool __cdecl dangerous_projectiles_near_player(int32* out_projectile_index)
{
	return INVOKE(0x00B77330, dangerous_projectiles_near_player, out_projectile_index);
}

//.text:00B77640 ; 
//.text:00B77650 ; 
//.text:00B77670 ; 
//.text:00B77680 ; void __cdecl projectile_accelerate(int32, const real_vector3d*, const real_vector3d*)
//.text:00B779B0 ; void __cdecl projectile_adjust_for_angular_velocity_change(int32)
//.text:00B77B30 ; bool __cdecl projectile_aim(int32, const real_point3d*, const real_point3d*, bool, real32*, real32*, real32*, real32*, bool, real_vector3d*, real32*, real32*, real32*, bool*)
//.text:00B77C00 ; bool __cdecl projectile_aim_ballistic(real32, real32, const real_point3d*, const real_point3d*, real32*, real32*, real32*, bool, real_vector3d*, real32*, real32*, real32*, real32*, real32*)
//.text:00B78170 ; bool __cdecl projectile_aim_linear(real32, const real_point3d*, const real_point3d*, real_vector3d*, real32*, real32*, real32*)
//.text:00B782A0 ; void __cdecl projectile_apply_detonation_damage(int32, const real_point3d*, const real_vector3d*, const real_vector3d*, real32, const s_damage_owner*, int32, s_location*, int16, bool, bool, bool, int32, e_predictability)
//.text:00B78430 ; void __cdecl projectile_apply_detonation_damage_locally(int32, const real_point3d*, const real_vector3d*, real32, int32, s_location*, bool, e_predictability)
//.text:00B78500 ; void __cdecl projectile_attach(int32, int32, int16, const real_point3d*, const s_location*)
//.text:00B78810 ; void __cdecl projectile_brute_grenade_update(int32)
//.text:00B78EB0 ; void __cdecl projectile_calculate_maximum_damage_distance(int32)
//.text:00B78F20 ; bool __cdecl projectile_cannot_be_affected_by_object_collision(int32)
//.text:00B78FD0 ; void __cdecl projectile_check_for_supercombine(int32)
//.text:00B79140 ; bool __cdecl projectile_collision(int32, collision_result*, real_point3d*, real_vector3d*, real_vector3d*, real32, int32, bool*)
//.text:00B7AD50 ; void __cdecl projectile_collision_build_fat_test_points(const real_point3d*, const real_point3d*, real32, int32, real_point3d*, real_point3d*)
//.text:00B7B130 ; bool __cdecl projectile_collision_test_line(int32, const real_point3d*, collision_result*)
//.text:00B7B460 ; bool __cdecl projectile_compute_function_value(int32, int32, int32, real32*, bool*, bool*)
//.text:00B7B5B0 ; int32 __cdecl projectile_count_attached_to_object_for_supercombine(int32, int32, int32*, int32*)
//.text:00B7B640 ; int32 __cdecl projectile_count_attached_to_object_for_supercombine_recursive(int32, int32, int32*)
//.text:00B7B700 ; bool __cdecl projectile_test_water(int32, const real_point3d*)
//.text:00B7B9B0 ; const s_projectile_material_response_definition* __cdecl projectile_definition_find_material_response(projectile_definition*, c_global_material_type)
//.text:00B7BA40 ; real32 __cdecl projectile_definition_get_autoaim_max_lead_time(int32)
//.text:00B7BA60 ; real32 __cdecl projectile_definition_get_velocity_multiplier(int32, bool)
//.text:00B7BB40 ; void __cdecl projectile_detonate(int32, int32, e_projectile_action_reasons)
//.text:00B7C370 ; void __cdecl projectile_detonate_effects_and_damage(int32, const real_point3d*, const real_vector3d*, const s_effect_vector*, real32, int32, c_flags<e_simulation_projectile_effect_flag, uns8, 6>, c_global_material_type, const real_vector3d*, s_location*, bool, bool, int32, e_predictability)
//.text:00B7C4D0 ; void __cdecl projectile_detonate_effects_and_damage_shared(int32, const real_point3d*, const real_vector3d*, const real_vector3d*, const s_effect_vector*, real32, const s_damage_owner*, int32, s_location*, int16, c_flags<e_simulation_projectile_effect_flag, uns8, 6>, c_global_material_type, bool, bool, bool, int32, e_predictability)
//.text:00B7C5A0 ; void __cdecl projectile_detonate_external(int32)
//.text:00B7C5C0 ; void __cdecl projectile_detonation_effect(int32, const real_point3d*, const real_vector3d*, const real_vector3d*, const s_damage_owner*, const s_damage_reporting_info&, bool, bool, const s_location*, c_global_material_type, const s_effect_vector*)
//.text:00B7C750 ; void __cdecl projectile_effect_new(int32, int32, int32, int32, s_effect_creation_data*, e_effect_deterministic)
//.text:00B7C7A0 ; bool __cdecl projectile_effected_by_phantom_volumes(int32)
//.text:00B7C7D0 ; bool __cdecl projectile_estimate_collision_along_arc(int32, real32, int32, s_collision_test_flags, bool, real_point3d*, real32*)
//.text:00B7CAA0 ; real32 __cdecl projectile_estimate_time_to_target(int32, real32, bool)
//.text:00B7CB00 ; real32 __cdecl projectile_get_ballistic_acceleration(int32)
//.text:00B7CB50 ; void __cdecl projectile_get_damage_reporting_info(int32, s_damage_reporting_info*)
//.text:00B7CBB0 ; void __cdecl projectile_get_effects(bool, const real_point3d*, const projectile_definition*, e_effect_type, c_global_material_type, s_material_effects_result*)
//.text:00B7CC20 ; void __cdecl projectile_get_effects_by_projectile(int32, const real_point3d*, const projectile_definition*, e_effect_type, c_global_material_type, bool, s_material_effects_result*)
//.text:00B7CCE0 ; void __cdecl projectile_get_target_point(int32, real_point3d*)
//.text:00B7D440 ; void __cdecl projectile_handle_deleted_object(int32, int32)
//.text:00B7D4A0 ; bool __cdecl projectile_handle_parent_destroyed(int32)
//.text:00B7D4F0 ; void __cdecl projectile_hide(int32)
//.text:00B7D530 ; const real_vector3d* __cdecl projectile_hit_normal_get(int32, real_vector3d*)
//.text:00B7D5C0 ; void __cdecl projectile_impact_detonation_effect(int32, real32, real32, const real_vector3d*, const real_point3d*, const real_vector3d*, c_global_material_type, bool, int32, int32, bool, bool)
//.text:00B7D7F0 ; void __cdecl projectile_initial_update(int32, bool, uns16, bool*, uns16*)
//.text:00B7DBD0 ; void __cdecl projectile_kill_tracer(int32)
//.text:00B7DC00 ; void __cdecl projectile_make_tracer(int32)
//.text:00B7DC30 ; void __cdecl projectile_material_detonation_effect(int32, const real_point3d*, const real_vector3d*, const s_damage_owner*, c_global_material_type, const real_vector3d*, const s_location*)
//.text:00B7DDB0 ; bool __cdecl projectile_new(int32, object_placement_data*, bool*)
//.text:00B7E160 ; void __cdecl projectile_orient_brute_grenade(int32, const real_point3d*, const real_plane3d*, int32, int32, int32, uns32)
//.text:00B7E3B0 ; void __cdecl projectile_set_action(int32, int16)
//.text:00B7E3F0 ; void __cdecl projectile_set_as_boarding(int32)
//.text:00B7E430 ; void __cdecl projectile_set_targeting_data(int32, const s_projectile_targeting_data*)
//.text:00B7E470 ; void __cdecl projectile_sound_detonation_effect(int32, const real_point3d*, const real_vector3d*)
//.text:00B7E500 ; int32 __cdecl projectile_supercombine_attached_projectiles_recursive(int32, int32, int32, real_point3d*)
//.text:00B7E610 ; void __cdecl projectile_targeting_data_get_ideal_point(const s_projectile_targeting_data*, real_point3d*)
//.text:00B7E760 ; void __cdecl projectile_targeting_data_get_target_velocity(const s_projectile_targeting_data*, real_vector3d*)
//.text:00B7E7B0 ; void __cdecl projectile_targeting_data_new(s_projectile_targeting_data*)
//.text:00B7E7C0 ; bool __cdecl projectile_targeting_data_valid(const s_projectile_targeting_data*)
//.text:00B7E800 ; void __cdecl projectile_translate_with_initial_test_against_object_type(int32, real_vector3d*)
//.text:00B7FA40 ; bool __cdecl projectile_update(int32)
//.text:00B7FBB0 ; void __cdecl projectiles_delete_all()
//.text:00B7FC00 ; void __cdecl projectiles_dispose()
//.text:00B7FC10 ; void __cdecl projectiles_dispose_from_old_map()
//.text:00B7FC20 ; void __cdecl projectiles_initialize()
//.text:00B7FC30 ; void __cdecl projectiles_initialize_for_new_map()
//.text:00B7FC40 ; void __cdecl projectiles_react_to_authoritative_detonation_on_player(int32, int32, bool)

void render_debug_projectiles()
{
	if (debug_projectiles)
	{
		// $IMPLEMENT
	}
}

