#include "units/units.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "game/players.hpp"
#include "items/equipment.hpp"
#include "items/equipment_definitions.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "motor/actions.hpp"
#include "objects/objects.hpp"
#include "physics/collisions.hpp"
#include "profiler/profiler.hpp"
#include "render/render_debug.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"
#include "units/unit_definition.hpp"
#include "units/unit_dialogue.hpp"
#include "units/vehicles.hpp"

#include <math.h>

HOOK_DECLARE_CALL(0x0053F212, unit_control); // player_submit_control
HOOK_DECLARE(0x00B47080, unit_render_debug);
HOOK_DECLARE(0x00B49F10, unit_update);

bool debug_objects_unit_vectors = false;
bool debug_objects_unit_seats = false;
bool debug_objects_unit_mouth_apeture = false;
bool debug_objects_unit_firing = false;
bool debug_objects_unit_melee = false;
int32 debug_unit_melee_unit_index = NONE;
bool debug_objects_unit_acceleration = false;
bool debug_objects_unit_camera = false;

bool debug_unit_illumination = false;
int32 debug_unit_illumination_time = NONE;

int32 __cdecl unit_get_current_primary_weapon(int32 unit_index)
{
	return INVOKE(0x0058C4D0, unit_get_current_primary_weapon, unit_index);
}

//.text:00B37710 ; 
//.text:00B37770 ; 

bool __cdecl any_unit_is_dangerous(int32* out_unit_index)
{
	return INVOKE(0x00B378F0, any_unit_is_dangerous, out_unit_index);
}

//.text:00B37990 ; public: void __cdecl c_multi_unit_passenger_iterator::begin(int32)
//.text:00B37A80 ; void __cdecl data_mine_add_player_death_with_killer_data(c_datamine*, int32, const s_damage_aftermath_result_data*)
//.text:00B37CE0 ; void __cdecl data_mine_insert_sp_actor_death(int32, const s_damage_aftermath_result_data*)
//.text:00B380B0 ; void __cdecl data_mine_insert_sp_player_death(int32, const s_damage_aftermath_result_data*)
//.text:00B38180 ; 
//.text:00B38210 ; public: bool __cdecl c_multi_unit_passenger_iterator::next()
//.text:00B38450 ; void __cdecl unit_arbiter_active_camouflage_play_sound(int32, int16)
//.text:00B386E0 ; bool __cdecl unit_acceleration_prevent_action(int32 unit_index)

bool __cdecl unit_active_camouflage_is_active(int32 unit_index)
{
	return INVOKE(0x00B38720, unit_active_camouflage_is_active, unit_index);
}

void __cdecl unit_active_camouflage_ding(int32 unit_index, real32 ding_amount, real32 transition_time)
{
	INVOKE(0x00B38760, unit_active_camouflage_ding, unit_index, ding_amount, transition_time);
}

void __cdecl unit_active_camouflage_disable(int32 unit_index, real32 transition_time)
{
	INVOKE(0x00B38880, unit_active_camouflage_disable, unit_index, transition_time);
}

void __cdecl unit_active_camouflage_enable(int32 unit_index, real32 transition_time, int32 end_time)
{
	INVOKE(0x00B38940, unit_active_camouflage_enable, unit_index, transition_time, end_time);
}

void __cdecl unit_active_camouflage_set_maximum(int32 unit_index, real32 maximum)
{
	INVOKE(0x00B38A10, unit_active_camouflage_set_maximum, unit_index, maximum);
}

void __cdecl unit_add_equipment_to_inventory(int32 unit_index, int32 slot_index, int32 equipment_index)
{
	INVOKE(0x00B38AB0, unit_add_equipment_to_inventory, unit_index, slot_index, equipment_index);
}

bool __cdecl unit_add_grenade_to_inventory(int32 unit_index, int32 equipment_index)
{
	return INVOKE(0x00B38BA0, unit_add_grenade_to_inventory, unit_index, equipment_index);
}

int16 __cdecl unit_add_grenade_type_to_inventory(int32 unit_index, int16 grenade_type, int16 grenade_count)
{
	return INVOKE(0x00B38CD0, unit_add_grenade_type_to_inventory, unit_index, grenade_type, grenade_count);
}

//.text:00B38D30 ; healthpack
//.text:00B38EA0 ; void __cdecl unit_add_initial_weapons(int32 unit_index)

void __cdecl unit_add_starting_profile_equipment(int32 unit_index, int16 starting_profile_index, bool reset, bool infinite_supply)
{
	INVOKE(0x00B39110, unit_add_starting_profile_equipment, unit_index, starting_profile_index, reset, infinite_supply);
}

bool __cdecl unit_add_weapon_to_inventory(int32 unit_index, int32 object_index, e_weapon_addition_method mode)
{
	return INVOKE(0x00B393D0, unit_add_weapon_to_inventory, unit_index, object_index, mode);
}

//.text:00B39820 ; bool __cdecl unit_adjust_first_person_camera(int32 unit_index, real_vector3d* aiming_vector)
//.text:00B399C0 ; void __cdecl unit_adjust_projectile_ray(int32 unit_index, real_point3d* origin, real_vector3d* forward, real_vector3d* inherited_velocity, const real_point3d* first_person_weapon_offset, const real_point3d* initial_good_position /* $TODO what is this actually named */, bool offset_origin, bool offset_aim, bool verify_origin, e_aiming_vector aiming_vector)
//.text:00B39E40 ; 
//.text:00B39EC0 ; void __cdecl unit_apply_predicted_weapon_state(int32 unit_index, int32 inventory_index)
//.text:00B39F60 ; void __cdecl unit_build_unit_interface_state(int32 unit_index, unit_interface_state* state)
//.text:00B3A040 ; bool __cdecl unit_calculate_is_in_illegal_position(int32 unit_index)

bool __cdecl unit_can_access_object(int32 unit_index, int32 object_index)
{
	return INVOKE(0x00B3A170, unit_can_access_object, unit_index, object_index);
}

//.text:00B3A660 ; bool __cdecl unit_can_enter_seat(int32 unit_index, int32 parent_unit_index, int16 seat_index, int32* occupant_unit_index_reference, bool* boarding_seat_reference, bool* banned_seat_reference)

bool __cdecl unit_can_pickup_equipment(int32 unit_index, int32 equipment_index)
{
	//return INVOKE(0x00B3AAA0, unit_can_pickup_equipment, unit_index, equipment_index);

	const equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
	const int32 current_equipment_index = unit_get_current_equipment(unit_index, 0);
	ASSERT(equipment_index != NONE && equipment_index != current_equipment_index);

	e_equipment_type equipment_type = equipment_definition_get_type(equipment->definition_index, 0);
	bool can_pickup = equipment_type == _equipment_type_multiplayer_powerup || VALID_INDEX(equipment_type, _equipment_type_treeoflife + 1) && equipment_remaining_charges(equipment_index);
	if (can_pickup && !equipment->item.inventory_state && !action_executing(unit_index, _action_equipment))
	{
		int32 player_index = UNIT_GET(unit_index)->unit.player_index;
		if (equipment_type == _equipment_type_multiplayer_powerup)
		{
			can_pickup = game_is_multiplayer() && game_engine_running();
		}
		else
		{
			if (current_equipment_index != NONE)
			{
				can_pickup = true;

				const equipment_datum* current_equipment = EQUIPMENT_GET(current_equipment_index);
				if (equipment->definition_index == current_equipment->definition_index)
				{
					if (equipment_remaining_charges(equipment_index) <= equipment_remaining_charges(current_equipment_index))
					{
						can_pickup = false;
					}
				}
			}

			if (can_pickup && player_index != NONE)
			{
				const player_datum* player = DATUM_GET(player_data, const player_datum, player_index);
				can_pickup = player->multiplayer.player_traits.get_weapons_traits()->get_weapon_pickup_allowed();
			}
		}
	}
	return can_pickup;
}

bool __cdecl unit_can_pickup_weapon(int32 unit_index, int32 weapon_index, e_weapon_addition_method mode, unit_weapon_pickup_result* result)
{
	return INVOKE(0x00B3AC10, unit_can_pickup_weapon, unit_index, weapon_index, mode, result);
}

bool __cdecl unit_can_see_point(int32 unit_index, const real_point3d* point, real32 theta)
{
	return INVOKE(0x00B3B190, unit_can_see_point, unit_index, point, theta);
}

//.text:00B3B360 ; bool __cdecl unit_can_use_seat_entry_marker(int32 unit_index, int32 parent_unit_index, int16 seat_index, const real_point3d* entry_marker_position, const real_vector3d* entry_marker_forward)
//.text:00B3B5B0 ; bool __cdecl unit_can_use_weapon(int32 unit_index, int32 weapon_index)
//.text:00B3B5D0 ; bool __cdecl unit_can_use_weapon_in_seat(int32 unit_index, int32 desired_seat, int32 weapon_index)
//.text:00B3B780 ; bool __cdecl unit_can_zoom(int32 unit_index)
//.text:00B3B920 ; void __cdecl unit_cause_ai_melee_attack(int32 unit_index, int16 melee_type)
//.text:00B3BD20 ; void __cdecl unit_cause_assassination_damage(int32, int32, const s_damage_owner*, int32, bool)
//.text:00B3BE30 ; void __cdecl unit_cause_melee_damage_response(int32 unit_index, int32 damage_definition_index, int32 damage_response_index, int32 target_object_index, c_global_material_type target_global_material_type, const real_point3d* collision_point, const real_vector3d* collision_normal)
//.text:00B3BFE0 ; void __cdecl unit_cause_player_melee_attack_apply(int32 unit_index, int32 unit_action, int16 melee_prediction_type, real32 scale, bool, s_player_melee_attack_query_result*, s_simulation_biped_melee_damage_event_data*)
//.text:00B3C360 ; unit_cause_player_melee_attack_query
//.text:00B3CDE0 ; void __cdecl unit_cause_player_melee_damage(int32 unit_index, int32 damage_definition_index, const s_damage_owner* damage_owner, const s_unit_player_melee_damage_target* damage_target)
//.text:00B3D3A0 ; void __cdecl unit_check_ping_thresholds(int32 unit_index, real32 shield_damage, real32 body_damage, int32* ping_type)
//.text:00B3D470 ; void __cdecl unit_clear_aim_assist(int32 unit_index)

bool __cdecl unit_clip_vector_to_aiming_screen_bounds(int32 unit_index, real_vector3d* vector, bool use_looking_screen)
{
	return INVOKE(0x00B3D4F0, unit_clip_vector_to_aiming_screen_bounds, unit_index, vector, use_looking_screen);
}

//.text:00B3D9A0 ; real32 __cdecl unit_compute_boost_fraction(int32 unit_index)
//.text:00B3DA70 ; bool __cdecl unit_compute_function_value(int32 unit_index, int32 function, int32 function_owner_definition_index, real32* value, bool* active, bool* deterministic)
//.text:00B3DDC0 ; int32 __cdecl unit_compute_ping_type(int32 unit_index, bool died, bool died_flying, bool died_airborne, bool ignore_hard_pings, bool force_hard_pings, bool ignore_hard_death, real32 shield_damage, real32 body_damage)
//.text:00B3DEE0 ; void __cdecl unit_consider_nearby_seats(int32 unit_index, int32 parent_unit_index, int32* best_unit_index, int16* best_seat_index, int16* best_seat_type, real32* best_seat_distance, bool* best_seat_driver)

void __cdecl unit_control(int32 unit_index, const unit_control_data* control_data)
{
	if (g_player_desired_mode_override != NONE)
	{
		((unit_control_data*)control_data)->desired_mode = g_player_desired_mode_override;
	}

	INVOKE(0x00B3E240, unit_control, unit_index, control_data);
}

//.text:00B3E3B0 ; void __cdecl unit_control_clear(unit_control_data* data)
//.text:00B3E440 ; bool __cdecl unit_controllable(int32 unit_index)
//.text:00B3E470 ; hologram related
//.text:00B3E4A0 ; hologram related
//.text:00B3E6D0 ; void __cdecl unit_correct_backpack_weapon_state(int32 unit_index)
//.text:00B3E780 ; void __cdecl unit_correct_dual_wield_state(int32 unit_index)
//.text:00B3E820 ; void __cdecl unit_create_children(int32 unit_index)
//.text:00B3E850 ; void __cdecl unit_create_reflexive_damage(int32 unit_index, int32 damage_effect_or_response_index, real32 scale)
//.text:00B3E9F0 ; int32 __cdecl unit_create_starting_weapon(int32 unit_index, const starting_weapon_info* weapon_info, bool infinite_supply)
//.text:00B3EAB0 ; void __cdecl unit_damage_aftermath_apply(int32, unit_index const s_damage_aftermath_result_data* result_data)
//.text:00B3F590 ; void __cdecl unit_damage_aftermath_compute(int32 unit_index, int32 being_damaged_flags, const s_damage_data* damage_data, s_damage_aftermath_result_data* result_data)

void __cdecl unit_debug_ninja_rope(int32 unit_index)
{
	//INVOKE(0x00B3F8C0, unit_debug_ninja_rope, unit_index);

	unit_datum* unit = UNIT_GET(unit_index);
	s_collision_test_flags flags{}; // 0x1001
	real_point3d camera_position{};
	real_vector3d aiming_vector{};
	collision_result collision{};

	flags.collision_flags.set(_collision_test_structure_bit, true);
	flags.collision_flags.set(_collision_test_back_facing_surfaces_bit, true);
	unit_get_camera_position(unit_index, &camera_position);
	scale_vector3d(&unit->unit.aiming_vector, 25.0f, &aiming_vector);
	int32 parent_index = object_get_ultimate_parent(unit_index);
	if (collision_test_vector(flags, &camera_position, &aiming_vector, parent_index, NONE, &collision))
	{
		if (collision.plane.n.k > 0.95f)
		{
			collision.point.z += 0.25f;
			object_set_position(parent_index, &collision.point, NULL, NULL, NULL);
		}
	}
}

//.text:00B3F9B0 ; void __cdecl unit_delete(int32 unit_index)
//.text:00B3FAD0 ; void __cdecl unit_delete_all_weapons(int32 unit_index)
//.text:00B3FB80 ; void __cdecl unit_delete_all_weapons_internal(int32 unit_index)

void __cdecl unit_delete_current_equipment(int32 unit_index, int32 slot_index)
{
	//INVOKE(0x00B3FC50, unit_delete_current_equipment, unit_index, slot_index);

	if (VALID_INDEX(slot_index, 4))
	{
		unit_datum* unit = UNIT_GET(unit_index);
		const int32 equipment_index = unit->unit.equipment_object_indices[slot_index];
		if (equipment_index != NONE)
		{
			unit_drop_item(unit_index, equipment_index, _unit_drop_type_delete);

			unit->unit.equipment_object_indices[slot_index] = NONE;
			unit->unit.active_equipment_object_indices[slot_index] = NONE;
			simulation_action_object_update(unit_index, _simulation_unit_update_equipment);

			if (unit->unit.actor_index != NONE)
			{
				actor_handle_equipment_delete(unit->unit.actor_index);
			}
		}
	}
}

//.text:00B3FD00 ; bool __cdecl unit_desires_blocked_tracking(int32 unit_index)
//.text:00B3FD60 ; bool __cdecl unit_desires_tight_camera_track(int32 unit_index)
//.text:00B3FD90 ; void __cdecl unit_destroy(int32 unit_index)
//.text:00B3FDE0 ; void __cdecl unit_detach_from_parent(int32 object_index)
//.text:00B40020 ; void __cdecl unit_detach_simulation_entity(int32 unit_index)
//.text:00B40080 ; void __cdecl unit_determine_melee_contest(int32 unit_index, int32 damage_definition_index, int32 clang_damage_definition_index, const s_damage_owner* damage_owner, const s_unit_player_melee_damage_target* damage_target, bool* out_contest, bool* out_clang_contest)
//.text:00B40230 ; void __cdecl unit_determine_melee_damage(int32 unit_index, int32 unit_action, int32* damage_definition_index, int32* damage_response_definition_index, int32* clang_damage_definition_index, int32* clang_damage_response_definition_index)
//.text:00B40410 ; void __cdecl unit_died(int32 unit_index, bool created_dead, bool about_to_be_destroyed)
//.text:00B40750 ; void __cdecl unit_dispose_from_old_structure_bsp(int32 unit_index)
//.text:00B407A0 ; bool __cdecl unit_does_not_show_readied_weapon(int32 unit_index)
//.text:00B407F0 ; bool __cdecl unit_driven_by_ai(int32 unit_index)
//.text:00B40840 ; void __cdecl unit_drop_all_hidden_inventory_weapons(int32 unit_index)

void __cdecl unit_drop_current_equipment(int32 unit_index, int32 slot_index)
{
	//INVOKE(0x00B409E0, unit_drop_current_equipment, unit_index, slot_index);

	if (VALID_INDEX(slot_index, 4))
	{
		unit_datum* unit = UNIT_GET(unit_index);
		const int32 equipment_index = unit->unit.equipment_object_indices[slot_index];
		if (equipment_index != NONE && !game_is_predicted())
		{
			if (equipment_remaining_charges(equipment_index))
			{
				const equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
				unit_drop_item(unit_index, equipment_index, _unit_drop_type_default);

				if (!equipment->object.physics_flags.test(_object_connected_to_physics_bit))
				{
					object_reconnect_to_physics(equipment_index);
				}

				unit->unit.equipment_object_indices[slot_index] = NONE;
				unit->unit.active_equipment_object_indices[slot_index] = NONE;
				simulation_action_object_update(unit_index, _simulation_unit_update_equipment);
			}
			else
			{
				unit_delete_current_equipment(unit_index, slot_index);
			}
		}
	}
}

void __cdecl unit_drop_item(int32 unit_index, int32 item_index, e_unit_drop_type drop_type)
{
	INVOKE(0x00B40A70, unit_drop_item, unit_index, item_index, drop_type);
}

//.text:00B40B80 ; bool __cdecl unit_equipment_desires_3rd_person_camera(int32)

// these should it in equipment.cpp
//.text:00B40BF0 ; bool __cdecl equipment_get_unit_camera(int32, const s_unit_camera**)
//.text:00B40CC0 ; equipment related

//.text:00B40D50 ; void __cdecl unit_estimate_position(int32 unit_index, int16 estimate_mode, const real_point3d* body_position, const real_vector3d* body_normal, const real_vector3d* desired_facing_vector, const real_vector3d* desired_gun_offset, real_point3d* estimated_position)
//.text:00B40EF0 ; void __cdecl unit_euler_aiming_update(const real_matrix4x3* orientation, real32 future_yaw, real_vector3d* aiming_vector, const real_vector3d* desired_aiming_vector, real32 dt, real_vector3d* aiming_velocity, const real_rectangle2d* input_aiming_bounds, real32 angular_velocity_limit, real32 angular_acceleration_limit, real32* out_unclipped_yaw)
//.text:00B42080 ; void __cdecl unit_euler_axis_buildplan(real32 current_location, real32 current_velocity, real32 velocity_limit, real32 acceleration_limit, euler_axis_plan* plan)
//.text:00B423A0 ; void __cdecl unit_euler_axis_couple(euler_axis_plan* plan1, euler_axis_plan* plan2, real32 velocity_limit, real32 acceleration_limit)
//.text:00B42570 ; bool __cdecl unit_euler_axis_doplan(const euler_axis_plan* plan, real32 delta_t, real32 current_location, real32* out_location, real32 current_velocity, real32* out_velocity)
//.text:00B42670 ; bool __cdecl unit_evict_parent_seat_occupant(int32 unit_index, bool kill_occupant, bool immediate)
//.text:00B427D0 ; void __cdecl unit_falling_damage_disable(int32 unit_index, bool disable)
//.text:00B42820 ; int16 __cdecl unit_find_nearby_seat(int32 unit_index, int32 parent_unit_index, int32* found_unit_index, int16* found_seat_index)
//.text:00B42890 ; int16 __cdecl unit_first_free_weapon_index(int32 unit_index)
//.text:00B428E0 ; bool __cdecl unit_fix_position(int32 unit_index, int32 line_of_site_object_index, const real_point3d* known_safe_location, const real_point3d* new_position, real_point3d* final_position, real32 minimum_xy_radius, real32 max_radius_fudge_factor, bool fix_below_new_position)
//.text:00B42BA0 ; void __cdecl unit_force_detach_from_parent_object(int32 unit_index)
//.text:00B42C50 ; void __cdecl unit_force_drop_weapon(int32 unit_index, int32 weapon_index, bool desire_new_weapon, bool suppress_network_event)

bool __cdecl unit_get_acceleration_origin_and_data(int32 unit_index, real_matrix4x3* origin, unit_seat_acceleration* acceleration_data)
{
	return INVOKE(0x00B42D20, unit_get_acceleration_origin_and_data, unit_index, origin, acceleration_data);
}

int32 __cdecl unit_get_active_primary_weapon(int32 unit_index, int32* gunner_unit_index_ref)
{
	return INVOKE(0x00B42EF0, unit_get_active_primary_weapon, unit_index, gunner_unit_index_ref);
}

//.text:00B42FD0 ; unit_get_additional_grenade_count?

int32 __cdecl unit_get_aim_assist_dash_target(int32 unit_index)
{
	return INVOKE(0x00B430B0, unit_get_aim_assist_dash_target, unit_index);
}

bool __cdecl unit_get_aim_position(int32 unit_index, real_point3d* aim_position)
{
	return INVOKE(0x00B43110, unit_get_aim_position, unit_index, aim_position);
}

int32 __cdecl unit_get_aiming_unit_index(int32 unit_index)
{
	return INVOKE(0x00B431F0, unit_get_aiming_unit_index, unit_index);
}

void __cdecl unit_get_aiming_vector(int32 unit_index, real_vector3d* aiming_vector)
{
	INVOKE(0x00B43300, unit_get_aiming_vector, unit_index, aiming_vector);
}

int16 __cdecl unit_get_all_seats(int32 unit_index, unit_seat_source* seat_array, int16 max_number_of_seats, bool allow_dead_vehicles)
{
	return INVOKE(0x00B43340, unit_get_all_seats, unit_index, seat_array, max_number_of_seats, allow_dead_vehicles);
}

//.text:00B43370 ; void __cdecl unit_get_all_seats_internal(int32 object_index, unit_seat_source* seat_array, int16* index, int16 max_number_of_seats, bool allow_dead_vehicles)
//.text:00B434C0 ; int32 __cdecl unit_get_best_seat_entrance_animation(int32 unit_index, int32 parent_unit_index, int16 seat_index, real_point3d* seat_marker_pos, real_point3d* animation_start_pos)
//.text:00B43630 ; bool __cdecl unit_get_body_position(int32 unit_index, real_point3d* body_position)
//.text:00B43670 ; void __cdecl unit_get_camera_info(int32 unit_index, s_unit_camera_info* camera_info)

void __cdecl unit_get_camera_position(int32 unit_index, real_point3d* camera_position)
{
	INVOKE(0x00B439D0, unit_get_camera_position, unit_index, camera_position);
}

//.text:00B43BB0 ; int32 __cdecl unit_get_current_backpack_weapon(int32 unit_index)

int32 __cdecl unit_get_current_equipment(int32 unit_index, int32 slot_index)
{
	//return INVOKE(0x00B43C20, unit_get_current_equipment, unit_index, slot_index);

	int32 equipment_index = NONE;
	if (VALID_INDEX(slot_index, 4))
	{
		const unit_datum* unit = UNIT_GET(unit_index);
		equipment_index = unit->unit.equipment_object_indices[slot_index];
	}
	return equipment_index;
}

//.text:00B43C70 ; 
//.text:00B43D20 ; int16 __cdecl unit_get_current_grenade_type(int32 unit_index)
//.text:00B43D50 ; int32 __cdecl unit_get_current_or_last_weak_player_index(int32 unit_index)
//.text:00B43D90 ; unit_seat* __cdecl unit_get_current_seat(int32 unit_index)
//.text:00B43E20 ; int16 __cdecl unit_get_driver_seat(int32 unit_index)
//.text:00B43EA0 ; unit_get_equipment_?
//.text:00B43F50 ; unit_get_equipment_?
//.text:00B43FF0 ; unit_get_equipment_?
//.text:00B44060 ; void __cdecl unit_get_facing_vector(int32 unit_index, real_vector3d* facing_vector)

real32 __cdecl unit_get_field_of_view(int32 unit_index, real32 default_field_of_view, int16 zoom_level)
{
	return INVOKE(0x00B44080, unit_get_field_of_view, unit_index, default_field_of_view, zoom_level);
}

//.text:00B440F0 ; int16 __cdecl unit_get_grenade_count(int32 unit_index, int16 grenade_type)
//.text:00B44140 ; bool __cdecl unit_get_gun_position(int32 unit_index, bool primary_trigger, real_point3d* gun_position)

void __cdecl unit_get_head_position(int32 unit_index, real_point3d* head_position)
{
	INVOKE(0x00B441B0, unit_get_head_position, unit_index, head_position);
}

//.text:00B441F0 ; unit_get_hologram_?
//.text:00B44250 ; unit_get_hologram_?
//.text:00B442D0 ; bool __cdecl unit_get_hologram_target_point(int32 unit_index, real_point3d* hologram_target_point)
//.text:00B44340 ; int32 __cdecl unit_get_last_associated_weak_player_index_for_appearance(int32 unit_index)
//.text:00B443B0 ; void __cdecl unit_get_looking_vector(int32 unit_index, real_vector3d* looking_vector)
//.text:00B443F0 ; real32 __cdecl unit_get_open_amount(int32 unit_index)
//.text:00B44470 ; c_sector_ref __cdecl unit_get_pathfinding_get_attached_sector(int32 unit_index)
//.text:00B444C0 ; e_game_team __cdecl unit_get_campaign_team(int32 unit_index)
//.text:00B444F0 ; int32 __cdecl unit_get_time_to_end_of_assasination(int32 unit_index)
//.text:00B445B0 ; int32 __cdecl unit_get_ultimate_motion_control_unit_index(int32 unit_index)
//.text:00B44600 ; int32 __cdecl unit_get_ultimate_weapon_control_unit_index(int32 unit_index)
//.text:00B44650 ; int32 __cdecl unit_get_weapon(const unit_datum* unit, int16 inventory_index)
//.text:00B44680 ; real32 __cdecl unit_get_weapon_dual_error_theta(int32 unit_index, int32 weapon_index)

int16 __cdecl unit_get_zoom_level(int32 unit_index)
{
	return INVOKE(0x00B44780, unit_get_zoom_level, unit_index);
}

//.text:00B447B0 ; real32 __cdecl unit_get_zoom_magnification(int32 unit_index, int16 zoom_level)
//.text:00B448C0 ; bool __cdecl unit_gunned_by_ai(int32 unit_index)
//.text:00B44910 ; void __cdecl unit_handle_deleted_object(int32 unit_index, int32 deleted_object_index)
//.text:00B44B60 ; void __cdecl unit_handle_deleted_player(int32 unit_index, int32deleted_player_index)
//.text:00B44BA0 ; 
//.text:00B44D60 ; void __cdecl unit_handle_region_destroyed(int32 unit_index, int16 region_index, uns32 region_damage_flags)
//.text:00B44D70 ; void __cdecl unit_handle_weapon_state_change(int32 unit_index, int32 weapon_index, int32 new_weapon_state)
//.text:00B44EF0 ; bool __cdecl equipment_suppresses_all_actions(int32)
//.text:00B44F90 ; 
//.text:00B44FC0 ; bool __cdecl unit_has_seat_animation(int32 unit_index, int32 vehicle_index, int16 seat_index)
//.text:00B45070 ; bool __cdecl unit_has_seat_entrance_animation(int32 unit_index, int32 parent_unit_index, int16 seat_index)

bool __cdecl unit_has_weapon_definition_index(int32 unit_index, int32 weapon_definition_index)
{
	return INVOKE(0x00B450F0, unit_has_weapon_definition_index, unit_index, weapon_definition_index);
}

//.text:00B45150 ; bool __cdecl unit_has_weapon_with_flag(int32 unit_index, e_weapon_flags flag)
//.text:00B451C0 ; bool __cdecl unit_in_third_person_seat(int32 unit_index)
//.text:00B45260 ; void __cdecl unit_inventory_cycle_weapon_set_identifier(int32 unit_index)
//.text:00B452F0 ; void __cdecl unit_inventory_drop_weapon(int32 unit_index, int16 inventory_index, e_unit_drop_type drop_type, bool desire_new_weapon)
//.text:00B45470 ; int16 __cdecl unit_inventory_get_must_be_readied_weapon(int32 unit_index)

int32 __cdecl unit_inventory_get_weapon(int32 unit_index, int16 inventory_index)
{
	return INVOKE(0x00B454D0, unit_inventory_get_weapon, unit_index, inventory_index);
}

//.text:00B45520 ; int16 __cdecl unit_inventory_next_grenade(int32 unit_index, int16 current_index, int16 delta)
//.text:00B455B0 ; int16 __cdecl unit_inventory_next_weapon(int32 unit_index, int16 current_index, int16 delta)
//.text:00B455D0 ; void __cdecl unit_inventory_set_weapon_index(int32 unit_index, int16 inventory_index, int32 weapon_index, e_unit_drop_type drop_type)
//.text:00B456A0 ; bool __cdecl unit_is_boosting(int32 unit_index)
//.text:00B456E0 ; bool __cdecl unit_is_busy(int32 unit_index)
//.text:00B456F0 ; bool __cdecl unit_is_dead_or_dying(int32 unit_index)
//.text:00B45750 ; bool __cdecl unit_is_dual_wielding(int32 unit_index)
//.text:00B457A0 ; bool __cdecl unit_is_dying(int32 unit_index)
//.text:00B457C0 ; bool __cdecl unit_is_fancy_assassinating(int32 unit_index)
//.text:00B457E0 ; bool __cdecl unit_is_flying_through_air(int32 unit_index)
//.text:00B45830 ; bool __cdecl unit_is_hard_pinging(int32 unit_index)
//.text:00B45860 ; bool __cdecl unit_is_in_entering_state(int32 unit_index)
//.text:00B458C0 ; bool __cdecl unit_is_in_seat_transition(int32 unit_index)
//.text:00B45930 ; bool __cdecl unit_is_overcharging_weapon(int32 unit_index)
//.text:00B459C0 ; bool __cdecl unit_is_player_motion_controlled(int32 unit_index)
//.text:00B45A40 ; bool __cdecl unit_is_soft_pinging(int32 unit_index)
//.text:00B45A50 ; bool __cdecl unit_is_sprinting(int32 unit_index)
//.text:00B45AB0 ; bool __cdecl unit_is_unarmed(int32 unit_index)
//.text:00B45B00 ; void __cdecl unit_kill(int32 unit_index)
//.text:00B45B40 ; void __cdecl unit_kill_hologram(int32 unit_index)
//.text:00B45B70 ; void __cdecl unit_kill_no_statistics(int32 unit_index)
//.text:00B45C00 ; void __cdecl unit_melee_effects(int32 unit_index, int32 damage_definition_index, int32 collision_object_index, e_sweetener_size sweetener_size, c_global_material_type global_material_type, const real_vector3d* melee_aiming_vector, const real_point3d* melee_collision_point, const real_vector3d* melee_collision_normal)
//.text:00B45EE0 ; bool __cdecl unit_new(int32 unit_index, object_placement_data* data, bool* out_of_memory)
//.text:00B464E0 ; void __cdecl unit_notify_of_tracking_or_locking(int32 unit_index, int32 tracking_object_index, int16 flags)
//.text:00B465A0 ; c_global_material_type __cdecl unit_override_damage_info_material_type(int32 object_index, c_global_material_type material_type)
//.text:00B46620 ; void __cdecl unit_passenger_iterator_new(int32 unit_index, unit_passenger_iterator* iterator)
//.text:00B46670 ; unit_datum* __cdecl unit_passenger_iterator_next(unit_passenger_iterator* iterator)
//.text:00B466F0 ; void __cdecl unit_pathfinding_attach(int32 unit_index, c_sector_ref sector_ref)
//.text:00B46730 ; void __cdecl unit_pathfinding_detach(int32 unit_index)
//.text:00B46780 ; bool __cdecl unit_pathfinding_is_attached(int32 unit_index)
//.text:00B467B0 ; void __cdecl unit_perform_detach_and_damage(int32 unit_index, int32 vehicle_index)
//.text:00B469E0 ; void __cdecl unit_persistent_control(int32 unit_index, int32 persistent_control_timer, uns32 persistent_control_flags)
//.text:00B46A20 ; void __cdecl unit_place(int32 unit_index, const s_scenario_unit_datum* scenario_unit)
//.text:00B46B20 ; bool __cdecl unit_placement_try_to_put_unit_in_seat_using_marker_name(int32 unit_index, int32 parent_index, int32 seat_marker_name)
//.text:00B46BF0 ; bool __cdecl unit_post_update(int32 unit_index)
//.text:00B46C10 ; void __cdecl unit_prepare_to_change_seats(int32 unit_index)
//.text:00B46C30 ; bool __cdecl unit_put_away_weapon(int32 unit_index, int16 weapon_set_index, bool immediate)
//.text:00B46D00 ; void __cdecl unit_record_damage(int32 unit_index, real32 damage, int16 damage_category, bool died, int32 damage_owner_player_index, c_game_team damage_owner_team_index, int32 damage_owner_object_index, const s_damage_reporting_info& damage_reporting_info)

void __cdecl unit_render_debug(int32 unit_index)
{
	//INVOKE(0x00B47080, unit_render_debug, unit_index);

	unit_datum* unit = UNIT_GET(unit_index);
	struct unit_definition* unit_definition = TAG_GET(UNIT_TAG, struct unit_definition, unit->definition_index);

	if (debug_objects_unit_vectors)
	{

	}

	if (debug_objects_unit_seats)
	{
		for (unit_seat& seat : unit_definition->unit.seats_block)
		{
			object_marker markers[4]{};
			int16 marker_count = object_get_markers_by_string_id(unit_index, seat.entry_markers_name.get_value(), markers, NUMBEROF(markers));
			for (int16 marker_index = 0; marker_index < marker_count; marker_index++)
			{
				object_marker* marker = &markers[marker_index];

				render_debug_string_at_point(&marker->matrix.position, string_id_get_string_const(seat.label.get_value()), global_real_argb_red);
				render_debug_vector(true, &marker->matrix.position, &marker->matrix.forward, seat.entry_radius, global_real_argb_red);
				render_debug_sphere(true, &marker->matrix.position, seat.entry_radius, global_real_argb_red);
				render_debug_cone_outline(true, &marker->matrix.position, &marker->matrix.forward, seat.entry_radius, seat.entry_marker_cone_angle, global_real_argb_red);
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

	if (debug_objects_unit_melee && debug_unit_melee_unit_index == unit_index)
	{

	}

	if (debug_objects_unit_acceleration)
	{
		real_matrix4x3 origin{};
		unit_seat_acceleration acceleration_data{};

		if (unit_get_acceleration_origin_and_data(unit_index, &origin, &acceleration_data))
		{
			object_marker markers[1]{};
			c_static_string<512> acceleration_string;

			object_get_markers_by_string_id(unit_index, STRING_ID(global, head), markers, NUMBEROF(markers));
			real_point3d point = markers[0].matrix.origin;

			real32 absolute_i = fabsf(unit->unit.seat_acceleration.i);
			real32 absolute_j = fabsf(unit->unit.seat_acceleration.j);
			real32 absolute_k = fabsf(unit->unit.seat_acceleration.k);
			real32 acceleration_scale = fmaxf(absolute_i, absolute_j);
			if (acceleration_scale <= absolute_k)
			{
				acceleration_scale = absolute_k;
			}

			const real_argb_color* acceleration_color = global_real_argb_blue;
			if (acceleration_data.action_limit != 0.0f && acceleration_scale >= acceleration_data.action_limit)
			{
				acceleration_color = global_real_argb_yellow;
			}
			if (acceleration_data.attachment_limit != 0.0f && acceleration_scale >= acceleration_data.attachment_limit)
			{
				acceleration_color = global_real_argb_red;
			}

			acceleration_string.print("accel= i:%.02f  j:%.02f  k:%.02f \r\nscale= %.02f\r\n",
				unit->unit.seat_acceleration.i,
				unit->unit.seat_acceleration.j,
				unit->unit.seat_acceleration.k,
				acceleration_scale),
			render_debug_string_at_point(&point, acceleration_string.get_string(), acceleration_color);
		}
	}

	if (debug_objects_unit_camera)
	{
		real_point3d unit_camera_position{};
		unit_get_camera_position(unit_index, &unit_camera_position);
		render_debug_point(true, &unit_camera_position, 0.2f, global_real_argb_cyan);
	}
}

void __cdecl unit_render_debug_indicator(int32 unit_index, const real_argb_color* color)
{
	// $IMPLEMENT
}

//.text:00B470A0 ; void __cdecl unit_reset_seat_acceleration_data(int32 unit_index)
//.text:00B470F0 ; void __cdecl unit_resolve_melee_attack(int32 unit_index, int32 damage_definition_index, int32 clang_damage_definition_index, const s_damage_owner* damage_owner, const s_unit_player_melee_damage_target* input_damage_target, bool melee_contest, bool clang_contest)
//.text:00B474C0 ; void __cdecl unit_respond_to_emp(int32 unit_index, bool applied_new_damage)
//.text:00B47630 ; int16 __cdecl unit_rotate_zoom_level(int32 unit_index, int16 zoom_level)
//.text:00B47770 ; void __cdecl unit_running_blind(int32 unit_index, real_vector3d* run_vector)
//.text:00B47A20 ; void __cdecl unit_seat_acceleration_clear(int32 unit_index, const real_point3d* position)
//.text:00B47AD0 ; void __cdecl unit_seat_acceleration_update(int32 unit_index)
//.text:00B47F80 ; bool __cdecl unit_seat_allow_noncombatants(int32 vehicle_index, int16 seat_index)
//.text:00B47FF0 ; bool __cdecl unit_seat_filled(int32 vehicle_index, int16 seat_index)
//.text:00B48010 ; int32 __cdecl unit_seat_get_occupant(int32 unit_index, int16 seat_index)
//.text:00B48080 ; bool __cdecl unit_seat_is_being_boarded(int32 unit_index, int16 destination_seat_index)
//.text:00B48140 ; bool __cdecl unit_seat_is_boarding(int32 unit_index, int16 seat_index)
//.text:00B48160 ; bool __cdecl unit_seat_is_driver(int32 unit_index, int16 seat_index)
//.text:00B48180 ; bool __cdecl unit_seat_is_gunner(int32 unit_index, int16 seat_index)
//.text:00B481A0 ; bool __cdecl unit_seat_is_leader(int32 unit_index, int16 seat_index)
//.text:00B481C0 ; bool __cdecl unit_seat_is_valid(int32 unit_index, int16 seat_index)
//.text:00B48240 ; bool __cdecl unit_seat_test_flag(int32 unit_index, int16 seat_index, e_unit_seat_flags flag)
//.text:00B482C0 ; void __cdecl unit_seat_try_closing(int32 unit_index, int32 seat_index, bool immediate, bool start_only, bool testing_early)
//.text:00B48380 ; void __cdecl unit_seat_try_opening(int32 unit_index, int32 seat_index, bool immediate, bool start_only, bool testing_late)
//.text:00B48440 ; 
//.text:00B484D0 ; void __cdecl unit_set_actively_controlled(int32 unit_index, bool actively_controlled)

void __cdecl unit_set_aiming_vectors(int32 unit_index, const real_vector3d* aiming_vector, const real_vector3d* looking_vector)
{
	INVOKE(0x00B48590, unit_set_aiming_vectors, unit_index, aiming_vector, looking_vector);
}

//.text:00B48610 ; void __cdecl unit_set_controllable(int32 unit_index, bool controllable)
//.text:00B48650 ; void __cdecl unit_set_desired_grenade_type(int32 unit_index, int16 grenade_type)
//.text:00B48690 ; void __cdecl unit_set_desired_weapon_set(int32 unit_index, const s_unit_weapon_set* weapon_set)
//.text:00B486D0 ; 
//.text:00B48710 ; void __cdecl unit_set_grenade_counts(int32 unit_index, const int8* grenade_counts)
//.text:00B48770 ; void __cdecl unit_set_hologram(int32 unit_index, int32, int32, const real_point3d*)
//.text:00B48980 ; void __cdecl unit_set_melee_attack_resolved(int32 unit_index)
//.text:00B489E0 ; void __cdecl unit_set_mouth_aperture(int32 unit_index, real32 aperture)
//.text:00B48A60 ; void __cdecl unit_set_player_melee_suppression(int32 unit_index, int32 recovery_ticks)
//.text:00B48AD0 ; void __cdecl unit_set_possessed(int32 unit_index, bool possessed)
//.text:00B48B20 ; void __cdecl unit_set_predicted_controller(int32 unit_index, int32 player_index, int32 simulation_actor_index, int32 simulation_actor_squad_index, int32 simulation_actor_fire_team_index, int32 simulation_actor_starting_location_index)
//.text:00B48BB0 ; void __cdecl unit_set_predicted_vehicle(int32 unit_index, int32 vehicle_index, int16 seat_index)
//.text:00B48C00 ; void __cdecl unit_set_prefer_tight_camera_track(int32 unit_index, bool setting)
//.text:00B48C40 ; void __cdecl unit_set_weapon_state(int32 unit_index, int32 inventory_index, int32 rounds_loaded, int32 rounds_inventory, real32 age)
//.text:00B48CB0 ; void __cdecl unit_set_weapon_type(int32 unit_index, int32 inventory_index, int32 weapon_definition_index, int16 multiplayer_weapon_identifier, int16 simulation_weapon_identifier, const s_unit_weapon_set* desired_weapon_set)
//.text:00B48EC0 ; bool __cdecl unit_should_clip_to_aiming_bounds(int32 unit_index)
//.text:00B48F10 ; void __cdecl unit_show_weapon_from_inventory(int32 weapon_index, int32 unit_index, e_unit_hand hand)
//.text:00B48FF0 ; void __cdecl unit_start_running_blindly(int32 unit_index)
//.text:00B49110 ; void __cdecl unit_stop_running_blindly(int32 unit_index)
//.text:00B49150 ; void __cdecl unit_swap_weapons_begin(int32 unit_index)
//.text:00B49170 ; void __cdecl unit_swap_weapons_end(int32 unit_index)
//.text:00B49190 ; void __cdecl unit_take_item(int32 unit_index, int32 item_index)
//.text:00B491B0 ; bool __cdecl unit_test_pickup_with_mode(int32 unit_index, const unit_weapon_pickup_result* pickup_result, e_weapon_addition_method mode, int16* motor_resource_mask)
//.text:00B492A0 ; bool __cdecl unit_test_seat_entrance_markers(int32 unit_index, int32 parent_unit_index, int16 seat_index)
//.text:00B49580 ; void __cdecl unit_toss_item(int32 unit_index, int32 item_index, int32 drop_marker, real32 toss_scale, bool toss_randomly)
//.text:00B49940 ; bool __cdecl unit_transfers_flashlight_value(int32 unit_index)
//.text:00B49990 ; bool __cdecl unit_player_plant_plasma_on_death(int32 unit_index, int32 player_index)

bool __cdecl unit_try_to_drop_weapon(int32 unit_index, bool drop_secondary)
{
	return INVOKE(0x00B49B00, unit_try_to_drop_weapon, unit_index, drop_secondary);
}

bool __cdecl unit_unsuspecting(int32 unit_index, const real_point3d* attacker_position, const real_vector3d* attacker_direction)
{
	return INVOKE(0x00B49BA0, unit_unsuspecting, unit_index, attacker_position, attacker_direction);
}

void __cdecl unit_unzoom(int32 unit_index, bool unzoom_binoculars_only)
{
	INVOKE(0x00B49D80, unit_unzoom, unit_index, unzoom_binoculars_only);
}

bool __cdecl unit_update(int32 unit_index)
{
	//return INVOKE(0x00B49F10, unit_update, unit_index);

	bool updated = false;
	PROFILER(unit_update)
	{
		//unit_verify_vectors(unit_index, "unit-update-begin");

		updated = unit_update_control(unit_index);
		updated |= unit_update_weapons(unit_index);
		updated |= unit_update_equipment(unit_index, 0);
		updated |= unit_update_aiming(unit_index);
		updated |= unit_update_seats(unit_index);
		updated |= unit_update_damage(unit_index);
		unit_dialogue_update(unit_index);
		unit_update_team_index(unit_index);
		unit_update_active_camouflage(unit_index);
		unit_update_vision_mode(unit_index);
		unit_update_illumination(unit_index);

		//unit_verify_vectors(unit_index, "unit-update-end");
	}
	return updated;
}

void __cdecl unit_update_active_camouflage(int32 unit_index)
{
	INVOKE(0x00B4A020, unit_update_active_camouflage, unit_index);
}

bool __cdecl unit_update_aiming(int32 unit_index)
{
	return INVOKE(0x00B4A2C0, unit_update_aiming, unit_index);
}

bool __cdecl unit_update_armor_lock(int32 unit_index)
{
	return INVOKE(0x00B4ABE0, unit_update_armor_lock, unit_index);
}

void __cdecl unit_update_consumable_energy(int32 unit_index)
{
	INVOKE(0x00B4AC70, unit_update_consumable_energy, unit_index);
}

bool __cdecl unit_update_control(int32 unit_index)
{
	return INVOKE(0x00B4AD30, unit_update_control, unit_index);
}

bool __cdecl unit_update_damage(int32 unit_index)
{
	return INVOKE(0x00B4B510, unit_update_damage, unit_index);
}

void __cdecl unit_update_driver_and_gunner(int32 unit_index)
{
	INVOKE(0x00B4B730, unit_update_active_camouflage, unit_index);
}

bool __cdecl unit_update_equipment(int32 unit_index, int32 slot_index)
{
	//return INVOKE(0x00B4BA20, unit_update_equipment, unit_index, slot_index);

	bool awake = false;

	const int32 equipment_index = unit_get_current_equipment(unit_index, slot_index);
	if (equipment_index != NONE)
	{
		if (equipment_remaining_charges(equipment_index) || equipment_active_fraction(equipment_index) != 0.0f)
		{
			equipment_update(equipment_index, unit_index);
		}
		else
		{
			unit_delete_current_equipment(unit_index, slot_index);
		}
	}

	return awake;
}

void __cdecl unit_update_health(int32 unit_index)
{
	INVOKE(0x00B4BA80, unit_update_health, unit_index);
}

void __cdecl unit_update_hologram(int32 unit_index)
{
	INVOKE(0x00B4BB90, unit_update_hologram, unit_index);
}

void __cdecl unit_update_illumination(int32 unit_index)
{
	if (debug_unit_illumination)
	{
		unit_datum* unit = UNIT_GET(unit_index);
		if (unit->unit.player_index != NONE)
		{
			int32 current_time = game_time_get();
			if (game_ticks_to_seconds(real32(current_time - debug_unit_illumination_time)) >= 1.0f)
			{
				event(_event_warning, "player illumination: self %.2f ambient %.2f",
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

bool __cdecl sub_B4BD70(int32 unit_index)
{
	return INVOKE(0x00B4BD70, sub_B4BD70, unit_index);

	//unit_datum* unit = UNIT_GET(unit_index);
	//if (unit->unit.__unknown3A8_object_index != NONE && !unit->unit.emp_timer)
	//{
	//	unit->unit.__unknown3A8_object_index = NONE;
	//}
	//
	//for (int32 parent_object_index = unit->object.parent_object_index; parent_object_index != NONE; parent_object_index = unit->object.parent_object_index)
	//{
	//	vehicle_datum* vehicle = VEHICLE_GET(parent_object_index);
	//	if (vehicle->unit.driver_object_index == unit_index && vehicle->unit.emp_timer > 0)
	//	{
	//		unit->unit.__unknown3A8_object_index = parent_object_index;
	//	}
	//}
	//
	//return unit->unit.__unknown3A8_object_index != NONE;
}

void __cdecl unit_update_predicted_controller(int32 unit_index)
{
	INVOKE(0x00B4BE30, unit_update_predicted_controller, unit_index);
}

void __cdecl unit_sync_with_predicted_vehicle(int32 unit_index)
{
	INVOKE(0x00B4BEF0, unit_sync_with_predicted_vehicle, unit_index);
}

bool __cdecl unit_update_seats(int32 unit_index)
{
	return INVOKE(0x00B4C020, unit_update_seats, unit_index);
}

void __cdecl unit_update_target_tracking(int32 unit_index)
{
	INVOKE(0x00B4C260, unit_update_target_tracking, unit_index);
}

void __cdecl unit_update_team_index(int32 unit_index)
{
	INVOKE(0x00B4C300, unit_update_team_index, unit_index);
}

void __cdecl unit_update_vision_mode(int32 unit_index)
{
	INVOKE(0x00B4C4A0, unit_update_vision_mode, unit_index);
}

bool __cdecl unit_update_weapons(int32 unit_index)
{
	return INVOKE(0x00B4C6D0, unit_update_weapons, unit_index);
}

bool __cdecl unit_use_current_equipment(int32 unit_index, int32 slot_index, bool network_predicted)
{
	return INVOKE(0x00B4CE90, unit_use_current_equipment, unit_index, slot_index, network_predicted);
}

void __cdecl sub_B4CF60(int32 unit_index, int32 equipment_definition_index)
{
	INVOKE(0x00B4CF60, sub_B4CF60, unit_index, equipment_definition_index);
}

bool __cdecl sub_B4D160(int32 unit_index)
{
	return INVOKE(0x00B4D160, sub_B4D160, unit_index);
}

real64 __cdecl sub_B4D1E0(int32 unit_index)
{
	return INVOKE(0x00B4D1E0, sub_B4D1E0, unit_index);
}

int16 __cdecl unit_weapon_next_index(int32 unit_index, int32 desired_seat, int16 current_index, int16 delta)
{
	return INVOKE(0x00B4D260, unit_weapon_next_index, unit_index, desired_seat, current_index, delta);
}

bool __cdecl units_debug_can_select_unit(int32 unit_index)
{
	//return INVOKE(0x00B4D3D0, units_debug_can_select_unit, unit_index);

	const object_header_datum* header = object_header_get(unit_index);
	void* object = NULL;
	if (header && TEST_BIT(_object_mask_unit, header->object_type.get()))
	{
		object = header->datum;
	}

	bool result = false;
	if (object)
	{
		unit_datum* unit = UNIT_GET(unit_index);
		result = unit->unit.player_index == NONE && !unit->object.damage_flags.test(_object_dead_bit) && !unit->object.flags.test(_object_created_with_parent_bit);
	}
	return result;
}

int32 __cdecl units_debug_get_closest_unit(int32 unit_index)
{
	//return INVOKE(0x00B4D430, units_debug_get_closest_unit, unit_index);

	int32 closest_unit_index = NONE;
	real32 closest_distance = k_real_max;

	c_object_iterator<unit_datum> unit_iterator;
	unit_iterator.begin(_object_mask_unit, 0);
	while (unit_iterator.next())
	{
		if (unit_iterator.get_index() != unit_index)
		{
			if (units_debug_can_select_unit(unit_iterator.get_index()))
			{
				real32 distance = 0.0f;
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

int32 __cdecl units_debug_get_next_unit(int32 unit_index)
{
	//return INVOKE(0x00B4D570, units_debug_get_next_unit, unit_index);

	c_object_iterator<unit_datum> unit_iterator;

	int32 next_unit_index = NONE;

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

