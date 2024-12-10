#include "items/projectiles.hpp"

bool debug_projectiles = false;

bool __cdecl dangerous_projectiles_near_player(long* out_projectile_index)
{
	return INVOKE(0x00B77330, dangerous_projectiles_near_player, out_projectile_index);
}

//.text:00B77640 ; 
//.text:00B77650 ; 
//.text:00B77670 ; 
//.text:00B77680 ; void __cdecl projectile_accelerate(long, real_vector3d const*, real_vector3d const*)
//.text:00B779B0 ; void __cdecl projectile_adjust_for_angular_velocity_change(long)
//.text:00B77B30 ; bool __cdecl projectile_aim(long, real_point3d const*, real_point3d const*, bool, real*, real*, real*, real*, bool, real_vector3d*, real*, real*, real*, bool*)
//.text:00B77C00 ; bool __cdecl projectile_aim_ballistic(real, real, real_point3d const*, real_point3d const*, real*, real*, real*, bool, real_vector3d*, real*, real*, real*, real*, real*)
//.text:00B78170 ; bool __cdecl projectile_aim_linear(real, real_point3d const*, real_point3d const*, real_vector3d*, real*, real*, real*)
//.text:00B782A0 ; void __cdecl projectile_apply_detonation_damage(long, real_point3d const*, real_vector3d const*, real_vector3d const*, real, s_damage_owner const*, long, s_location*, short, bool, bool, bool, long, e_predictability)
//.text:00B78430 ; void __cdecl projectile_apply_detonation_damage_locally(long, real_point3d const*, real_vector3d const*, real, long, s_location*, bool, e_predictability)
//.text:00B78500 ; void __cdecl projectile_attach(long, long, short, real_point3d const*, s_location const*)
//.text:00B78810 ; void __cdecl projectile_brute_grenade_update(long)
//.text:00B78EB0 ; void __cdecl projectile_calculate_maximum_damage_distance(long)
//.text:00B78F20 ; bool __cdecl projectile_cannot_be_affected_by_object_collision(long)
//.text:00B78FD0 ; void __cdecl projectile_check_for_supercombine(long)
//.text:00B79140 ; bool __cdecl projectile_collision(long, collision_result*, real_point3d*, real_vector3d*, real_vector3d*, real, long, bool*)
//.text:00B7AD50 ; void __cdecl projectile_collision_build_fat_test_points(real_point3d const*, real_point3d const*, real, long, real_point3d*, real_point3d*)
//.text:00B7B130 ; bool __cdecl projectile_collision_test_line(long, real_point3d const*, collision_result*)
//.text:00B7B460 ; bool __cdecl projectile_compute_function_value(long, long, long, real*, bool*, bool*)
//.text:00B7B5B0 ; long __cdecl projectile_count_attached_to_object_for_supercombine(long, long, long*, long*)
//.text:00B7B640 ; long __cdecl projectile_count_attached_to_object_for_supercombine_recursive(long, long, long*)
//.text:00B7B700 ; bool __cdecl projectile_test_water(long, real_point3d const*)
//.text:00B7B9B0 ; s_projectile_material_response_definition const* __cdecl projectile_definition_find_material_response(projectile_definition*, c_global_material_type)
//.text:00B7BA40 ; real __cdecl projectile_definition_get_autoaim_max_lead_time(long)
//.text:00B7BA60 ; real __cdecl projectile_definition_get_velocity_multiplier(long, bool)
//.text:00B7BB40 ; void __cdecl projectile_detonate(long, long, e_projectile_action_reasons)
//.text:00B7C370 ; void __cdecl projectile_detonate_effects_and_damage(long, real_point3d const*, real_vector3d const*, s_effect_vector const*, real, long, c_flags<e_simulation_projectile_effect_flag, byte, 6>, c_global_material_type, real_vector3d const*, s_location*, bool, bool, long, e_predictability)
//.text:00B7C4D0 ; void __cdecl projectile_detonate_effects_and_damage_shared(long, real_point3d const*, real_vector3d const*, real_vector3d const*, s_effect_vector const*, real, s_damage_owner const*, long, s_location*, short, c_flags<e_simulation_projectile_effect_flag, byte, 6>, c_global_material_type, bool, bool, bool, long, e_predictability)
//.text:00B7C5A0 ; void __cdecl projectile_detonate_external(long)
//.text:00B7C5C0 ; void __cdecl projectile_detonation_effect(long, real_point3d const*, real_vector3d const*, real_vector3d const*, s_damage_owner const*, s_damage_reporting_info const&, bool, bool, s_location const*, c_global_material_type, s_effect_vector const*)
//.text:00B7C750 ; void __cdecl projectile_effect_new(long, long, long, long, s_effect_creation_data*, e_effect_deterministic)
//.text:00B7C7A0 ; bool __cdecl projectile_effected_by_phantom_volumes(long)
//.text:00B7C7D0 ; bool __cdecl projectile_estimate_collision_along_arc(long, real, long, s_collision_test_flags, bool, real_point3d*, real*)
//.text:00B7CAA0 ; real __cdecl projectile_estimate_time_to_target(long, real, bool)
//.text:00B7CB00 ; real __cdecl projectile_get_ballistic_acceleration(long)
//.text:00B7CB50 ; void __cdecl projectile_get_damage_reporting_info(long, s_damage_reporting_info*)
//.text:00B7CBB0 ; void __cdecl projectile_get_effects(bool, real_point3d const*, projectile_definition const*, e_effect_type, c_global_material_type, s_material_effects_result*)
//.text:00B7CC20 ; void __cdecl projectile_get_effects_by_projectile(long, real_point3d const*, projectile_definition const*, e_effect_type, c_global_material_type, bool, s_material_effects_result*)
//.text:00B7CCE0 ; void __cdecl projectile_get_target_point(long, real_point3d*)
//.text:00B7D440 ; void __cdecl projectile_handle_deleted_object(long, long)
//.text:00B7D4A0 ; bool __cdecl projectile_handle_parent_destroyed(long)
//.text:00B7D4F0 ; void __cdecl projectile_hide(long)
//.text:00B7D530 ; real_vector3d const* __cdecl projectile_hit_normal_get(long, real_vector3d*)
//.text:00B7D5C0 ; void __cdecl projectile_impact_detonation_effect(long, real, real, real_vector3d const*, real_point3d const*, real_vector3d const*, c_global_material_type, bool, long, long, bool, bool)
//.text:00B7D7F0 ; void __cdecl projectile_initial_update(long, bool, word, bool*, word*)
//.text:00B7DBD0 ; void __cdecl projectile_kill_tracer(long)
//.text:00B7DC00 ; void __cdecl projectile_make_tracer(long)
//.text:00B7DC30 ; void __cdecl projectile_material_detonation_effect(long, real_point3d const*, real_vector3d const*, s_damage_owner const*, c_global_material_type, real_vector3d const*, s_location const*)
//.text:00B7DDB0 ; bool __cdecl projectile_new(long, object_placement_data*, bool*)
//.text:00B7E160 ; void __cdecl projectile_orient_brute_grenade(long, real_point3d const*, real_plane3d const*, long, long, long, dword)
//.text:00B7E3B0 ; void __cdecl projectile_set_action(long, short)
//.text:00B7E3F0 ; void __cdecl projectile_set_as_boarding(long)
//.text:00B7E430 ; void __cdecl projectile_set_targeting_data(long, s_projectile_targeting_data const*)
//.text:00B7E470 ; void __cdecl projectile_sound_detonation_effect(long, real_point3d const*, real_vector3d const*)
//.text:00B7E500 ; long __cdecl projectile_supercombine_attached_projectiles_recursive(long, long, long, real_point3d*)
//.text:00B7E610 ; void __cdecl projectile_targeting_data_get_ideal_point(s_projectile_targeting_data const*, real_point3d*)
//.text:00B7E760 ; void __cdecl projectile_targeting_data_get_target_velocity(s_projectile_targeting_data const*, real_vector3d*)
//.text:00B7E7B0 ; void __cdecl projectile_targeting_data_new(s_projectile_targeting_data*)
//.text:00B7E7C0 ; bool __cdecl projectile_targeting_data_valid(s_projectile_targeting_data const*)
//.text:00B7E800 ; void __cdecl projectile_translate_with_initial_test_against_object_type(long, real_vector3d*)
//.text:00B7FA40 ; bool __cdecl projectile_update(long)
//.text:00B7FBB0 ; void __cdecl projectiles_delete_all()
//.text:00B7FC00 ; void __cdecl projectiles_dispose()
//.text:00B7FC10 ; void __cdecl projectiles_dispose_from_old_map()
//.text:00B7FC20 ; void __cdecl projectiles_initialize()
//.text:00B7FC30 ; void __cdecl projectiles_initialize_for_new_map()
//.text:00B7FC40 ; void __cdecl projectiles_react_to_authoritative_detonation_on_player(long, long, bool)

void render_debug_projectiles()
{
	if (debug_projectiles)
	{
		// #TODO: implement
	}
}

