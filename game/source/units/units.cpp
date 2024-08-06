#include "units/units.hpp"

#include "cache/cache_files.hpp"
#include "game/players.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "objects/objects.hpp"
#include "physics/collisions.hpp"
#include "profiler/profiler.hpp"
#include "render/render_debug.hpp"
#include "units/unit_definition.hpp"
#include "units/unit_dialogue.hpp"
#include "units/vehicles.hpp"

HOOK_DECLARE_CALL(0x0053F212, unit_control); // player_submit_control
HOOK_DECLARE(0x00B47080, unit_render_debug);
HOOK_DECLARE(0x00B49F10, unit_update);

bool debug_objects_unit_vectors = false;
bool debug_objects_unit_seats = false;
bool debug_objects_unit_mouth_apeture = false;
bool debug_objects_unit_firing = false;
bool debug_objects_unit_acceleration = false;
bool debug_objects_unit_camera = false;

bool debug_unit_illumination = false;
long debug_unit_illumination_time = NONE;

long __cdecl unit_get_current_primary_weapon(long unit_index)
{
	return INVOKE(0x0058C4D0, unit_get_current_primary_weapon, unit_index);
}

//.text:00B37710 ; 
//.text:00B37770 ; 

bool __cdecl any_unit_is_dangerous(long* out_unit_index)
{
	return INVOKE(0x00B378F0, any_unit_is_dangerous, out_unit_index);
}

//.text:00B37990 ; public: void __cdecl c_multi_unit_passenger_iterator::begin(long)
//.text:00B37A80 ; void __cdecl data_mine_add_player_death_with_killer_data(c_datamine*, long, s_damage_aftermath_result_data const*)
//.text:00B37CE0 ; void __cdecl data_mine_insert_sp_actor_death(long, s_damage_aftermath_result_data const*)
//.text:00B380B0 ; void __cdecl data_mine_insert_sp_player_death(long, s_damage_aftermath_result_data const*)
//.text:00B38180 ; 
//.text:00B38210 ; public: bool __cdecl c_multi_unit_passenger_iterator::next()
//.text:00B38450 ; void __cdecl unit_arbiter_active_camouflage_play_sound(long, short)
//.text:00B386E0 ; bool __cdecl unit_acceleration_prevent_action(long)

bool __cdecl unit_active_camouflage_is_active(long unit_index)
{
	return INVOKE(0x00B38720, unit_active_camouflage_is_active, unit_index);
}

void __cdecl unit_active_camouflage_ding(long unit_index, real active_camo_ding, real active_camo_regrowth_rate)
{
	INVOKE(0x00B38760, unit_active_camouflage_ding, unit_index, active_camo_ding, active_camo_regrowth_rate);
}

void __cdecl unit_active_camouflage_disable(long unit_index, real interpolation_time)
{
	INVOKE(0x00B38880, unit_active_camouflage_disable, unit_index, interpolation_time);
}

void __cdecl unit_active_camouflage_enable(long unit_index, real interpolation_time, long end_time)
{
	INVOKE(0x00B38940, unit_active_camouflage_enable, unit_index, interpolation_time, end_time);
}

void __cdecl unit_active_camouflage_strength(long unit_index, real active_camouflage)
{
	INVOKE(0x00B38A10, unit_active_camouflage_strength, unit_index, active_camouflage);
}

void __cdecl unit_add_equipment_to_inventory(long unit_index, long slot_index, long object_index)
{
	INVOKE(0x00B38AB0, unit_add_equipment_to_inventory, unit_index, slot_index, object_index);
}

bool __cdecl unit_add_grenade_to_inventory(long unit_index, long object_index)
{
	return INVOKE(0x00B38BA0, unit_add_grenade_to_inventory, unit_index, object_index);
}

short __cdecl unit_add_grenade_type_to_inventory(long unit_index, short grenade_type, short grenade_count)
{
	return INVOKE(0x00B38CD0, unit_add_grenade_type_to_inventory, unit_index, grenade_type, grenade_count);
}

//.text:00B38D30 ; healthpack
//.text:00B38EA0 ; void __cdecl unit_add_initial_weapons(long)

void __cdecl unit_add_starting_profile_equipment(long unit_index, short profile_index, bool clear_player, bool create_new)
{
	INVOKE(0x00B39110, unit_add_starting_profile_equipment, unit_index, profile_index, clear_player, create_new);
}

bool __cdecl unit_add_weapon_to_inventory(long unit_index, long object_index, long weapon_addition_method)
{
	return INVOKE(0x00B393D0, unit_add_weapon_to_inventory, unit_index, object_index, weapon_addition_method);
}

//.text:00B39820 ; bool __cdecl unit_adjust_first_person_camera(long, vector3d*)
//.text:00B399C0 ; void __cdecl unit_adjust_projectile_ray(long, real_point3d*, vector3d*, vector3d*, real_point3d const*, real_point3d const*, bool, bool, bool, e_aiming_vector)
//.text:00B39E40 ; 
//.text:00B39EC0 ; 
//.text:00B39F60 ; void __cdecl unit_build_unit_interface_state(long, unit_interface_state*)
//.text:00B3A040 ; bool __cdecl unit_calculate_is_in_illegal_position(long)
//.text:00B3A170 ; bool __cdecl unit_can_access_object(long, long)
//.text:00B3A660 ; bool __cdecl unit_can_enter_seat(long, long, short, long*, bool*, bool*)
//.text:00B3AAA0 ; bool __cdecl unit_can_pickup_equipment(long, long)
//.text:00B3AC10 ; bool __cdecl unit_can_pickup_weapon(long, long, e_weapon_addition_method, unit_weapon_pickup_result*)
//.text:00B3B190 ; bool __cdecl unit_can_see_point(long, real_point3d const*, real)
//.text:00B3B360 ; bool __cdecl unit_can_use_seat_entry_marker(long, long, short, real_point3d const*, vector3d const*)
//.text:00B3B5B0 ; bool __cdecl unit_can_use_weapon(long, long)
//.text:00B3B5D0 ; bool __cdecl unit_can_use_weapon_in_seat(long, long, long)
//.text:00B3B780 ; bool __cdecl unit_can_zoom(long)
//.text:00B3B920 ; void __cdecl unit_cause_ai_melee_attack(long, short)
//.text:00B3BD20 ; void __cdecl unit_cause_assassination_damage(long, long, s_damage_owner const*, long, bool)
//.text:00B3BE30 ; void __cdecl unit_cause_melee_damage_response(long, long, long, long, c_global_material_type, real_point3d const*, vector3d const*)
//.text:00B3BFE0 ; void __cdecl unit_cause_player_melee_attack_apply(long, long, short, real, bool, s_player_melee_attack_query_result*, s_simulation_biped_melee_damage_event_data*)
//.text:00B3C360 ; unit_cause_player_melee_attack_query
//.text:00B3CDE0 ; void __cdecl unit_cause_player_melee_damage(long, long, s_damage_owner const*, s_unit_player_melee_damage_target const*)
//.text:00B3D3A0 ; void __cdecl unit_check_ping_thresholds(long, real, real, long*)
//.text:00B3D470 ; void __cdecl unit_clear_aim_assist(long)
//.text:00B3D4F0 ; bool __cdecl unit_clip_vector_to_aiming_screen_bounds(long, vector3d*, bool)
//.text:00B3D9A0 ; real __cdecl unit_compute_boost_fraction(long)
//.text:00B3DA70 ; bool __cdecl unit_compute_function_value(long, long, long, real*, bool*, bool*, bool)
//.text:00B3DDC0 ; long __cdecl unit_compute_ping_type(long, bool, bool, bool, bool, bool, bool, real, real)
//.text:00B3DEE0 ; void __cdecl unit_consider_nearby_seats(long, long, long*, short*, short*, real*, bool*)

void __cdecl unit_control(long unit_index, unit_control_data const* control_data)
{
	if (g_player_desired_mode_override != NONE)
		const_cast<unit_control_data*>(control_data)->desired_mode = g_player_desired_mode_override;

	INVOKE(0x00B3E240, unit_control, unit_index, control_data);
}

//.text:00B3E3B0 ; void __cdecl unit_control_clear(unit_control_data*)
//.text:00B3E440 ; bool __cdecl unit_controllable(long)
//.text:00B3E470 ; hologram related
//.text:00B3E4A0 ; hologram related
//.text:00B3E6D0 ; void __cdecl unit_correct_backpack_weapon_state(long)
//.text:00B3E780 ; void __cdecl unit_correct_dual_wield_state(long)
//.text:00B3E820 ; void __cdecl unit_create_children(long)
//.text:00B3E850 ; void __cdecl unit_create_reflexive_damage(long, long, real)
//.text:00B3E9F0 ; long __cdecl unit_create_starting_weapon(long, starting_weapon_info const*, bool)
//.text:00B3EAB0 ; void __cdecl unit_damage_aftermath_apply(long, s_damage_aftermath_result_data const*)
//.text:00B3F590 ; void __cdecl unit_damage_aftermath_compute(long, long, s_damage_data const*, s_damage_aftermath_result_data*)

void __cdecl unit_debug_ninja_rope(long unit_index)
{
	//INVOKE(0x00B3F8C0, unit_debug_ninja_rope, unit_index);

	unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
	s_collision_test_flags flags{}; // 0x1001
	real_point3d camera_position{};
	vector3d aiming_vector{};
	collision_result collision{};

	flags.collision_flags.set(_collision_test_structure_bit, true);
	flags.collision_flags.set(_collision_test_back_facing_surfaces_bit, true);
	unit_get_camera_position(unit_index, &camera_position);
	scale_vector3d(&unit->unit.aiming_vector, 25.0f, &aiming_vector);
	long parent_index = object_get_ultimate_parent(unit_index);
	if (collision_test_vector(flags, &camera_position, &aiming_vector, parent_index, NONE, &collision))
	{
		if (collision.plane.n.k > 0.95f)
		{
			collision.position.z += 0.25f;
			object_set_position(parent_index, &collision.position, NULL, NULL, NULL);
		}
	}
}

//.text:00B3F9B0 ; void __cdecl unit_delete(long)
//.text:00B3FAD0 ; void __cdecl unit_delete_all_weapons(long)
//.text:00B3FB80 ; void __cdecl unit_delete_all_weapons_internal(long)
//.text:00B3FC50 ; void __cdecl unit_delete_current_equipment(long, long)
//.text:00B3FD00 ; bool __cdecl unit_desires_blocked_tracking(long)
//.text:00B3FD60 ; bool __cdecl unit_desires_tight_camera_track(long)
//.text:00B3FD90 ; void __cdecl unit_destroy(long)
//.text:00B3FDE0 ; void __cdecl unit_detach_from_parent(long)
//.text:00B40020 ; void __cdecl unit_detach_simulation_entity(long)
//.text:00B40080 ; void __cdecl unit_determine_melee_contest(long, long, long, s_damage_owner const*, s_unit_player_melee_damage_target const*, bool*, bool*)
//.text:00B40230 ; void __cdecl unit_determine_melee_damage(long, long, long*, long*, long*, long*)
//.text:00B40410 ; void __cdecl unit_died(long, bool, bool)
//.text:00B40750 ; void __cdecl unit_dispose_from_old_structure_bsp(long)
//.text:00B407A0 ; bool __cdecl unit_does_not_show_readied_weapon(long)
//.text:00B407F0 ; bool __cdecl unit_driven_by_ai(long)
//.text:00B40840 ; void __cdecl unit_drop_all_hidden_inventory_weapons(long)
//.text:00B409E0 ; void __cdecl unit_drop_current_equipment(long, long)
//.text:00B40A70 ; void __cdecl unit_drop_item(long, long, e_unit_drop_type)
//.text:00B40B80 ; bool __cdecl unit_equipment_desires_3rd_person_camera(long)

// these should it in equipment.cpp
//.text:00B40BF0 ; bool __cdecl equipment_get_unit_camera(long, s_unit_camera const**)
//.text:00B40CC0 ; equipment related

//.text:00B40D50 ; void __cdecl unit_estimate_position(long, short, real_point3d const*, vector3d const*, vector3d const*, vector3d const*, real_point3d*)
//.text:00B40EF0 ; void __cdecl unit_euler_aiming_update(real_matrix4x3 const*, real, vector3d*, vector3d const*, real, vector3d*, real_rectangle2d const*, real, real, real*)
//.text:00B42080 ; void __cdecl unit_euler_axis_buildplan(real, real, real, real, euler_axis_plan*)
//.text:00B423A0 ; void __cdecl unit_euler_axis_couple(euler_axis_plan*, euler_axis_plan*, real, real)
//.text:00B42570 ; bool __cdecl unit_euler_axis_doplan(euler_axis_plan const*, real, real, real*, real, real*)
//.text:00B42670 ; bool __cdecl unit_evict_parent_seat_occupant(long, bool, bool)
//.text:00B427D0 ; void __cdecl unit_falling_damage_disable(long, bool)
//.text:00B42820 ; short __cdecl unit_find_nearby_seat(long, long, long*, short*)
//.text:00B42890 ; short __cdecl unit_first_free_weapon_index(long)
//.text:00B428E0 ; bool __cdecl unit_fix_position(long, long, real_point3d const*, real_point3d const*, real_point3d*, real, real, bool)
//.text:00B42BA0 ; void __cdecl unit_force_detach_from_parent_object(long)
//.text:00B42C50 ; void __cdecl unit_force_drop_weapon(long, long, bool, bool)
//.text:00B42D20 ; bool __cdecl unit_get_acceleration_origin_and_data(long, real_matrix4x3*, unit_seat_acceleration*)

long __cdecl unit_get_active_primary_weapon(long unit_index, long* parent_unit_index)
{
	return INVOKE(0x00B42EF0, unit_get_active_primary_weapon, unit_index, parent_unit_index);
}

//.text:00B42FD0 ; unit_get_max_grenades_modifier_value?

long __cdecl unit_get_aim_assist_dash_target(long unit_index)
{
	return INVOKE(0x00B430B0, unit_get_aim_assist_dash_target, unit_index);
}

bool __cdecl unit_get_aim_position(long unit_index, real_point3d* aim_position)
{
	return INVOKE(0x00B43110, unit_get_aim_position, unit_index, aim_position);
}

long __cdecl unit_get_aiming_unit_index(long unit_index)
{
	return INVOKE(0x00B431F0, unit_get_aiming_unit_index, unit_index);
}

void __cdecl unit_get_aiming_vector(long unit_index, vector3d* aiming_vector)
{
	INVOKE(0x00B43300, unit_get_aiming_vector, unit_index, aiming_vector);
}

short __cdecl unit_get_all_seats(long unit_index, unit_seat_source* sources, short maximum_source_count, bool a4)
{
	return INVOKE(0x00B43340, unit_get_all_seats, unit_index, sources, maximum_source_count, a4);
}

//.text:00B43370 ; void __cdecl unit_get_all_seats_internal(long, unit_seat_source*, short*, short, bool)
//.text:00B434C0 ; long __cdecl unit_get_best_seat_entrance_animation(long, long, short, real_point3d*, real_point3d*)
//.text:00B43630 ; bool __cdecl unit_get_body_position(long, real_point3d*)
//.text:00B43670 ; void __cdecl unit_get_camera_info(long, s_unit_camera_info*)

void __cdecl unit_get_camera_position(long unit_index, real_point3d* position)
{
	INVOKE(0x00B439D0, unit_get_camera_position, unit_index, position);
}

//.text:00B43BB0 ; long __cdecl unit_get_current_backpack_weapon(long)
//.text:00B43C20 ; long __cdecl unit_get_current_equipment(long, long)
//.text:00B43C70 ; 
//.text:00B43D20 ; short __cdecl unit_get_current_grenade_type(long)
//.text:00B43D50 ; long __cdecl unit_get_current_or_last_weak_player_index(long)
//.text:00B43D90 ; unit_seat* __cdecl unit_get_current_seat(long)
//.text:00B43E20 ; short __cdecl unit_get_driver_seat(long)
//.text:00B43EA0 ; 
//.text:00B43F50 ; 
//.text:00B43FF0 ; 
//.text:00B44060 ; void __cdecl unit_get_facing_vector(long, vector3d*)

real __cdecl unit_get_field_of_view(long unit_index, real fov_radians, short zoom_level)
{
	return INVOKE(0x00B44080, unit_get_field_of_view, unit_index, fov_radians, zoom_level);
}

//.text:00B440F0 ; short __cdecl unit_get_grenade_count(long, short)
//.text:00B44140 ; bool __cdecl unit_get_gun_position(long, bool, real_point3d*)

void __cdecl unit_get_head_position(long unit_index, real_point3d* position)
{
	INVOKE(0x00B441B0, unit_get_head_position, unit_index, position);
}

//.text:00B441F0 ; 
//.text:00B44250 ; 
//.text:00B442D0 ; bool __cdecl unit_get_hologram_target_point(long, real_point3d*)
//.text:00B44340 ; long __cdecl unit_get_last_associated_weak_player_index_for_appearance(long)
//.text:00B443B0 ; void __cdecl unit_get_looking_vector(long, vector3d*)
//.text:00B443F0 ; real __cdecl unit_get_open_amount(long)
//.text:00B44470 ; c_sector_ref __cdecl unit_get_pathfinding_get_attached_sector(long)
//.text:00B444C0 ; e_campaign_team __cdecl unit_get_campaign_team(long)
//.text:00B444F0 ; long __cdecl unit_get_time_to_end_of_assasination(long)
//.text:00B445B0 ; long __cdecl unit_get_ultimate_motion_control_unit_index(long)
//.text:00B44600 ; long __cdecl unit_get_ultimate_weapon_control_unit_index(long)
//.text:00B44650 ; 
//.text:00B44680 ; 

short __cdecl unit_get_zoom_level(long unit_index)
{
	return INVOKE(0x00B44780, unit_get_zoom_level, unit_index);
}

//.text:00B447B0 ; real __cdecl unit_get_zoom_magnification(long, short)
//.text:00B448C0 ; 
//.text:00B44910 ; void __cdecl unit_handle_deleted_object(long, long)
//.text:00B44B60 ; void __cdecl unit_handle_deleted_player(long, long)
//.text:00B44BA0 ; 
//.text:00B44D60 ; void __cdecl unit_handle_region_destroyed(long, short, dword)
//.text:00B44D70 ; void __cdecl unit_handle_weapon_state_change(long, long, long)
//.text:00B44EF0 ; bool __cdecl equipment_suppresses_all_actions(long)
//.text:00B44F90 ; 
//.text:00B44FC0 ; bool __cdecl unit_has_seat_animation(long, long, short)
//.text:00B45070 ; bool __cdecl unit_has_seat_entrance_animation(long, long, short)

bool __cdecl unit_has_weapon_definition_index(long unit_index, long weapon_definition_index)
{
	return INVOKE(0x00B450F0, unit_has_weapon_definition_index, unit_index, weapon_definition_index);
}

//.text:00B45150 ; 
//.text:00B451C0 ; bool __cdecl unit_in_third_person_seat(long)
//.text:00B45260 ; void __cdecl unit_inventory_cycle_weapon_set_identifier(long)
//.text:00B452F0 ; void __cdecl unit_inventory_drop_weapon(long, short, e_unit_drop_type, bool)
//.text:00B45470 ; short __cdecl unit_inventory_get_must_be_readied_weapon(long)
//.text:00B454D0 ; long __cdecl unit_inventory_get_weapon(long, short)
//.text:00B45520 ; short __cdecl unit_inventory_next_grenade(long, short, short)
//.text:00B455B0 ; short __cdecl unit_inventory_next_weapon(long, short, short)
//.text:00B455D0 ; void __cdecl unit_inventory_set_weapon_index(long, short, long, e_unit_drop_type)
//.text:00B456A0 ; bool __cdecl unit_is_boosting(long)
//.text:00B456E0 ; bool __cdecl unit_is_busy(long)
//.text:00B456F0 ; bool __cdecl unit_is_dead_or_dying(long)
//.text:00B45750 ; bool __cdecl unit_is_dual_wielding(long)
//.text:00B457A0 ; bool __cdecl unit_is_dying(long)
//.text:00B457C0 ; 
//.text:00B457E0 ; bool __cdecl unit_is_flying_through_air(long)
//.text:00B45830 ; bool __cdecl unit_is_hard_pinging(long)
//.text:00B45860 ; bool __cdecl unit_is_in_entering_state(long)
//.text:00B458C0 ; bool __cdecl unit_is_in_seat_transition(long)
//.text:00B45930 ; bool __cdecl unit_is_overcharging_weapon(long)
//.text:00B459C0 ; bool __cdecl unit_is_player_motion_controlled(long)
//.text:00B45A40 ; bool __cdecl unit_is_soft_pinging(long)
//.text:00B45A50 ; bool __cdecl unit_is_sprinting(long)
//.text:00B45AB0 ; bool __cdecl unit_is_unarmed(long)
//.text:00B45B40 ; unit_kill_hologram?
//.text:00B45B70 ; void __cdecl unit_kill_no_statistics(long)
//.text:00B45C00 ; unit_melee_effects
//.text:00B45C00 ; void __cdecl unit_melee_effects(long, long, long, e_sweetener_size, c_global_material_type, vector3d const*, real_point3d const*, vector3d const*, bool)
//.text:00B45EE0 ; bool __cdecl unit_new(long, object_placement_data*, bool*)
//.text:00B464E0 ; void __cdecl unit_notify_of_tracking_or_locking(long, long, short)
//.text:00B465A0 ; unit_override_damage_info_material_type
//.text:00B46620 ; void __cdecl unit_passenger_iterator_new(long, unit_passenger_iterator*)
//.text:00B46670 ; unit_datum* __cdecl unit_passenger_iterator_next(unit_passenger_iterator*)
//.text:00B466F0 ; void __cdecl unit_pathfinding_attach(long, c_sector_ref)
//.text:00B46730 ; void __cdecl unit_pathfinding_detach(long)
//.text:00B46780 ; bool __cdecl unit_pathfinding_is_attached(long)
//.text:00B467B0 ; void __cdecl unit_perform_detach_and_damage(long, long)
//.text:00B469E0 ; void __cdecl unit_persistent_control(long, long, dword)
//.text:00B46A20 ; void __cdecl unit_place(long, s_scenario_unit_datum const*)
//.text:00B46B20 ; bool __cdecl unit_placement_try_to_put_unit_in_seat_using_marker_name(long, long, long)
//.text:00B46BF0 ; bool __cdecl unit_post_update(long)
//.text:00B46C10 ; void __cdecl unit_prepare_to_change_seats(long)
//.text:00B46C30 ; bool __cdecl unit_put_away_weapon(long, short, bool)
//.text:00B46D00 ; void __cdecl unit_record_damage(long, real, short, bool, long, c_game_team, long, s_damage_reporting_info const&)

void __cdecl unit_render_debug(long unit_index)
{
	//INVOKE(0x00B47080, unit_render_debug, unit_index);

	unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
	struct unit_definition* unit_definition = (struct unit_definition*)tag_get(UNIT_TAG, unit->definition_index);

	if (debug_objects_unit_vectors)
	{

	}

	if (debug_objects_unit_seats)
	{
		for (unit_seat& seat : unit_definition->unit.seats_block)
		{
			object_marker markers[4]{};
			short marker_count = object_get_markers_by_string_id(unit_index, seat.entry_markers_name.get_value(), markers, NUMBEROF(markers));
			for (short marker_index = 0; marker_index < marker_count; marker_index++)
			{
				object_marker* marker = &markers[marker_index];

				render_debug_string_at_point(&marker->node_matrix.position, string_id_get_string_const(seat.label.get_value()), global_real_argb_red);
				render_debug_vector(true, &marker->node_matrix.position, &marker->node_matrix.forward, seat.entry_radius, global_real_argb_red);
				render_debug_sphere(true, &marker->node_matrix.position, seat.entry_radius, global_real_argb_red);
				render_debug_cone_outline(true, &marker->node_matrix.position, &marker->node_matrix.forward, seat.entry_radius, seat.entry_marker_cone_angle, global_real_argb_red);
			}
		}
	}

	if (debug_objects_unit_mouth_apeture)
	{
		real_point3d head_position{};
		c_static_string<512> mouth_apeture_string;

		unit_get_head_position(unit_index, &head_position);
		mouth_apeture_string.print("%.2f", unit->unit.mouth_aperture);

		render_debug_string_at_point(&head_position, mouth_apeture_string.get_string(), global_real_argb_orange);
	}

	if (debug_objects_unit_firing)
	{

	}

	if (debug_objects_unit_acceleration)
	{

	}

	if (debug_objects_unit_camera)
	{
		real_point3d unit_camera_position{};
		unit_get_camera_position(unit_index, &unit_camera_position);
		render_debug_point(true, &unit_camera_position, 0.2f, global_real_argb_cyan);
	}
}

//.text:00B470A0 ; void __cdecl unit_reset_seat_acceleration_data(long)
//.text:00B470F0 ; unit_resolve_melee_attack
//.text:00B474C0 ; void __cdecl unit_respond_to_emp(long, bool)
//.text:00B47630 ; short __cdecl unit_rotate_zoom_level(long, short)
//.text:00B47770 ; void __cdecl unit_running_blind(long, vector3d*)
//.text:00B47A20 ; 
//.text:00B47AD0 ; 
//.text:00B47F80 ; 
//.text:00B47FF0 ; bool __cdecl unit_seat_filled(long, short)
//.text:00B48010 ; long __cdecl unit_seat_get_occupant(long, short)
//.text:00B48080 ; bool __cdecl unit_seat_is_being_boarded(long, short)
//.text:00B48140 ; bool __cdecl unit_seat_is_boarding(long, short)
//.text:00B48160 ; bool __cdecl unit_seat_is_driver(long, short)
//.text:00B48180 ; bool __cdecl unit_seat_is_gunner(long, short)
//.text:00B481A0 ; bool __cdecl unit_seat_is_leader(long, short)
//.text:00B481C0 ; bool __cdecl unit_seat_is_valid(long, short)
//.text:00B48240 ; bool __cdecl unit_seat_test_flag(long, short, e_unit_seat_flags)
//.text:00B482C0 ; void __cdecl unit_seat_try_closing(long, long, bool, bool, bool)
//.text:00B48380 ; void __cdecl unit_seat_try_opening(long, long, bool, bool, bool)
//.text:00B48440 ; 
//.text:00B484D0 ; void __cdecl unit_set_actively_controlled(long, bool)
//.text:00B48590 ; void __cdecl unit_set_aiming_vectors(long, vector3d const*, vector3d const*)
//.text:00B48610 ; void __cdecl unit_set_controllable(long, bool)
//.text:00B48650 ; void __cdecl unit_set_desired_grenade_type(long, short)
//.text:00B48690 ; void __cdecl unit_set_desired_weapon_set(long, s_unit_weapon_set const*)
//.text:00B486D0 ; 
//.text:00B48710 ; void __cdecl unit_set_grenade_counts(long, char const*)
//.text:00B48770 ; void __cdecl unit_set_hologram(long, long, long, real_point3d const*)
//.text:00B48980 ; void __cdecl unit_set_melee_attack_resolved(long)
//.text:00B489E0 ; void __cdecl unit_set_mouth_aperture(long, real)
//.text:00B48A60 ; void __cdecl unit_set_player_melee_suppression(long, long)
//.text:00B48AD0 ; void __cdecl unit_set_possessed(long, bool)
//.text:00B48B20 ; void __cdecl unit_set_predicted_controller(long, long, long, long, long, long)
//.text:00B48BB0 ; void __cdecl unit_set_predicted_vehicle(long, long, short)
//.text:00B48C00 ; void __cdecl unit_set_prefer_tight_camera_track(long, bool)
//.text:00B48C40 ; void __cdecl unit_set_weapon_state(long, long, long, long, real)
//.text:00B48CB0 ; void __cdecl unit_set_weapon_type(long, long, long, short, byte, s_unit_weapon_set const*)
//.text:00B48EC0 ; bool __cdecl unit_should_clip_to_aiming_bounds(long)
//.text:00B48F10 ; void __cdecl unit_show_weapon_from_inventory(long, long, e_unit_hand)
//.text:00B48FF0 ; void __cdecl unit_start_running_blindly(long unit_index)
//.text:00B49110 ; void __cdecl unit_stop_running_blindly(long unit_index)
//.text:00B49150 ; void __cdecl unit_swap_weapons_begin(long unit_index)
//.text:00B49170 ; void __cdecl unit_swap_weapons_end(long unit_index)
//.text:00B49190 ; void __cdecl unit_take_item(long, long)
//.text:00B491B0 ; bool __cdecl unit_test_pickup_with_mode(long, unit_weapon_pickup_result const*, e_weapon_addition_method, short*)
//.text:00B492A0 ; 
//.text:00B49580 ; void __cdecl unit_toss_item(long unit_index, long item_index, long marker_name, real a4, bool a5)
//.text:00B49940 ; bool __cdecl unit_transfers_flashlight_value(long unit_index)
//.text:00B49990 ; bool __cdecl unit_player_plant_plasma_on_death(long unit_index, long player_index)

bool __cdecl unit_try_to_drop_weapon(long unit_index, bool drop_secondary)
{
	return INVOKE(0x00B49B00, unit_try_to_drop_weapon, unit_index, drop_secondary);
}

bool __cdecl unit_unsuspecting(long unit_index, real_point3d const* attacker_position, vector3d const* attacker_direction)
{
	return INVOKE(0x00B49BA0, unit_unsuspecting, unit_index, attacker_position, attacker_direction);
}

void __cdecl unit_unzoom(long unit_index, bool a2)
{
	INVOKE(0x00B49D80, unit_unzoom, unit_index, a2);
}

bool __cdecl unit_update(long unit_index)
{
	//return INVOKE(0x00B49F10, unit_update, unit_index);

	bool updated = false;
	PROFILER(unit_update)
	{
		//unit_verify_vectors(unit_index, "unit-update-begin");

		updated = unit_update_control(unit_index);
		updated |= unit_update_weapons(unit_index);

		for (long slot_index = 0; slot_index < 4; slot_index++)
			unit_update_equipment(unit_index, slot_index);

		updated |= unit_update_aiming(unit_index);
		updated |= unit_update_seats(unit_index);
		updated |= unit_update_damage(unit_index);

		// HO
		updated |= sub_B4BD70(unit_index);
		unit_update_consumable_energy(unit_index);

		unit_dialogue_update(unit_index);
		unit_update_team_index(unit_index);
		unit_update_active_camouflage(unit_index);
		unit_update_health(unit_index);
		unit_update_vision_mode(unit_index);
		unit_update_illumination(unit_index);
		unit_update_hologram(unit_index);

		// HO
		unit_update_armor_lock(unit_index);
		unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
		sub_B4BCB0(&unit->unit.__unknown2FC);
		sub_B4BCB0(&unit->unit.__unknown310);

		//unit_verify_vectors(unit_index, "unit-update-end");
	}
	return updated;
}

void __cdecl unit_update_active_camouflage(long unit_index)
{
	INVOKE(0x00B4A020, unit_update_active_camouflage, unit_index);
}

bool __cdecl unit_update_aiming(long unit_index)
{
	return INVOKE(0x00B4A2C0, unit_update_aiming, unit_index);
}

bool __cdecl unit_update_armor_lock(long unit_index)
{
	return INVOKE(0x00B4ABE0, unit_update_armor_lock, unit_index);
}

void __cdecl unit_update_consumable_energy(long unit_index)
{
	INVOKE(0x00B4AC70, unit_update_consumable_energy, unit_index);
}

bool __cdecl unit_update_control(long unit_index)
{
	return INVOKE(0x00B4AD30, unit_update_control, unit_index);
}

bool __cdecl unit_update_damage(long unit_index)
{
	return INVOKE(0x00B4B510, unit_update_damage, unit_index);
}

void __cdecl unit_update_driver_and_gunner(long unit_index)
{
	INVOKE(0x00B4B730, unit_update_active_camouflage, unit_index);
}

bool __cdecl unit_update_equipment(long unit_index, long slot_index)
{
	return INVOKE(0x00B4BA20, unit_update_equipment, unit_index, slot_index);

	// HO
	//if (VALID_INDEX(slot_index, 4))
	//{
	//	long current_equipment_index = ((unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit))->unit.equipment_object_indices[slot_index];
	//	if (current_equipment_index != NONE)
	//		sub_B891F0(current_equipment_index, unit_index);
	//}

	// H3
	//long current_equipment_index = ((unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit))->unit.equipment_object_indices[slot_index];
	//if (equipment_remaining_charges(current_equipment_index) || equipment_active_fraction(current_equipment_index) != 0.0f)
	//{
	//	sub_B891F0(current_equipment_index, unit_index);
	//}
	//else
	//{
	//	unit_delete_current_equipment(unit_index);
	//}
	//
	//return false
}

void __cdecl unit_update_health(long unit_index)
{
	INVOKE(0x00B4BA80, unit_update_health, unit_index);
}

void __cdecl unit_update_hologram(long unit_index)
{
	INVOKE(0x00B4BB90, unit_update_hologram, unit_index);
}

void __cdecl unit_update_illumination(long unit_index)
{
	if (debug_unit_illumination)
	{
		unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
		if (unit->unit.player_index != NONE)
		{
			long current_time = game_time_get();
			if (game_ticks_to_seconds(real(current_time - debug_unit_illumination_time)) >= 1.0f)
			{
				generate_event(_event_level_warning, "player illumination: self %.2f ambient %.2f",
					unit->unit.self_illumination,
					unit->unit.ambient_illumination);

				debug_unit_illumination_time = current_time;
			}
		}
	}
}

void __cdecl sub_B4BCB0(s_unknown_unit_struct_sizeof_14* a1)
{
	INVOKE(0x00B4BCB0, sub_B4BCB0, a1);
}

bool __cdecl sub_B4BD70(long unit_index)
{
	return INVOKE(0x00B4BD70, sub_B4BD70, unit_index);

	//unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
	//if (unit->unit.__unknown3A8_object_index != NONE && !unit->unit.emp_timer)
	//	unit->unit.__unknown3A8_object_index = NONE;
	//
	//for (long parent_object_index = unit->object.parent_object_index; parent_object_index != NONE; parent_object_index = unit->object.parent_object_index)
	//{
	//	vehicle_datum* vehicle = (vehicle_datum*)object_get_and_verify_type(parent_object_index, _object_mask_vehicle);
	//	if (vehicle->unit.driver_object_index == unit_index && vehicle->unit.emp_timer > 0)
	//		unit->unit.__unknown3A8_object_index = parent_object_index;
	//}
	//
	//return unit->unit.__unknown3A8_object_index != NONE;
}

void __cdecl unit_update_predicted_controller(long unit_index)
{
	INVOKE(0x00B4BE30, unit_update_predicted_controller, unit_index);
}

void __cdecl unit_sync_with_predicted_vehicle(long unit_index)
{
	INVOKE(0x00B4BEF0, unit_sync_with_predicted_vehicle, unit_index);
}

bool __cdecl unit_update_seats(long unit_index)
{
	return INVOKE(0x00B4C020, unit_update_seats, unit_index);
}

void __cdecl unit_update_target_tracking(long unit_index)
{
	INVOKE(0x00B4C260, unit_update_target_tracking, unit_index);
}

void __cdecl unit_update_team_index(long unit_index)
{
	INVOKE(0x00B4C300, unit_update_team_index, unit_index);
}

void __cdecl unit_update_vision_mode(long unit_index)
{
	INVOKE(0x00B4C4A0, unit_update_vision_mode, unit_index);
}

bool __cdecl unit_update_weapons(long unit_index)
{
	return INVOKE(0x00B4C6D0, unit_update_weapons, unit_index);
}

bool __cdecl unit_use_current_equipment(long unit_index, long slot_index, bool a3)
{
	return INVOKE(0x00B4CE90, unit_use_current_equipment, unit_index, slot_index, a3);
}

void __cdecl sub_B4CF60(long unit_index, long equipment_definition_index)
{
	INVOKE(0x00B4CF60, sub_B4CF60, unit_index, equipment_definition_index);
}

bool __cdecl sub_B4D160(long unit_index)
{
	return INVOKE(0x00B4D160, sub_B4D160, unit_index);
}

double __cdecl sub_B4D1E0(long unit_index)
{
	return INVOKE(0x00B4D1E0, sub_B4D1E0, unit_index);
}

short __cdecl unit_weapon_next_index(long unit_index, long a2, short a3, short a4)
{
	return INVOKE(0x00B4D260, unit_weapon_next_index, unit_index, a2, a3, a4);
}

bool __cdecl units_debug_can_select_unit(long unit_index)
{
	//return INVOKE(0x00B4D3D0, units_debug_can_select_unit, unit_index);

	object_header_datum const* header = object_header_get(unit_index);
	void* object = nullptr;
	if (header && TEST_BIT(_object_mask_unit, header->object_type.get()))
		object = header->datum;

	if (!object)
		return false;

	unit_datum* unit = (unit_datum*)object_try_and_get_and_verify_type(unit_index, _object_mask_unit);
	return unit->unit.player_index == NONE && !TEST_BIT(unit->object.damage_flags, 2) && !unit->object.flags.test(_object_created_with_parent_bit);
}

long __cdecl units_debug_get_closest_unit(long unit_index)
{
	//return INVOKE(0x00B4D430, units_debug_get_closest_unit, unit_index);

	long closest_unit_index = NONE;
	real closest_distance = k_real_max;

	c_object_iterator<unit_datum> unit_iterator;
	unit_iterator.begin(_object_mask_unit, 0);
	while (unit_iterator.next())
	{
		if (unit_iterator.get_index() != unit_index)
		{
			if (units_debug_can_select_unit(unit_iterator.get_index()))
			{
				real distance = 0.0f;
				if (unit_index != NONE)
				{
					real_point3d unit_origin{};
					object_get_origin(unit_index, &unit_origin);
					real_point3d closest_unit_origin{};
					object_get_origin(unit_iterator.get_index(), &closest_unit_origin);
					distance = distance3d(&unit_origin, &closest_unit_origin);
				}

				if (distance < closest_distance)
				{
					closest_unit_index = unit_iterator.get_index();
					closest_distance = distance;
				}
			}
		}
	}

	return closest_unit_index;
}

long __cdecl units_debug_get_next_unit(long unit_index)
{
	//return INVOKE(0x00B4D570, units_debug_get_next_unit, unit_index);

	c_object_iterator<unit_datum> unit_iterator;

	long next_unit_index = NONE;

	if (unit_index != NONE)
	{
		unit_iterator.begin(_object_mask_unit, 0);
		if (unit_iterator.next())
		{
			while (unit_iterator.get_index() != unit_index && unit_iterator.next());
		}
		if (unit_iterator.next())
		{
			while (true)
			{
				if (units_debug_can_select_unit(unit_iterator.get_index()))
					break;

				if (!unit_iterator.next())
					goto LABEL_12;
			}

			next_unit_index = unit_iterator.get_index();
			if (unit_iterator.get_index() != -1)
				return next_unit_index;
		}
	}
LABEL_12:
	unit_iterator.begin(_object_mask_unit, 0);
	if (!unit_iterator.next())
		return next_unit_index;

	while (true)
	{
		if (units_debug_can_select_unit(unit_iterator.get_index()))
			break;

		if (!unit_iterator.next())
			return next_unit_index;
	}
	return unit_iterator.get_index();
}

