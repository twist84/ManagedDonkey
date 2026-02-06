#include "units/bipeds.hpp"

#include "ai/ai.hpp"
#include "cache/cache_files.hpp"
#include "cutscene/recorded_animations.hpp"
#include "game/cheats.hpp"
#include "math/periodic_functions.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "motor/motor_animation.hpp"
#include "motor/motor_system.hpp"
#include "physics/character_physics.hpp"
#include "render/render_debug.hpp"
#include "units/biped_definitions.hpp"

#include <math.h>

REFERENCE_DECLARE(0x01947EAC, real32, global_lean_pivot_offset) = 1.3f;
REFERENCE_DECLARE(0x01947EB0, real32, global_lean_return_amount) = 0.6f;
REFERENCE_DECLARE(0x01947EFC, real32, global_lean_angle) = 20.0f * DEG;
REFERENCE_DECLARE(0x01947F00, real32, global_lean_turn_angle) = 30.0f * DEG;

HOOK_DECLARE(0x00B6B8F0, biped_bumped_object);
HOOK_DECLARE(0x00B6CC70, biped_compute_lean);
HOOK_DECLARE(0x00B70DF0, biped_render_debug);
HOOK_DECLARE(0x00B716C0, biped_update);

bool debug_objects_physics_control_node = false;
bool debug_objects_biped_autoaim_pills = false;
bool debug_objects_ground_plane = false;
bool debug_objects_movement_mode = false;
bool debug_biped_throttle = false;
bool debug_objects_unit_pathfinding_surface = false;
bool debug_objects_pendulum = false;

//.text:00B6A2F0 ; void __cdecl biped_adjust_node_matrices(int32, int32, real_matrix4x3*)
//.text:00B6AF70 ; void __cdecl biped_adjust_placement(object_placement_data*)
//.text:00B6AFE0 ; bool __cdecl biped_approximate_surface_index(int32, int32, int32, real_point3d*, int16*, int32*, int32*, int32*, uns32*)
//.text:00B6B0A0 ; void __cdecl biped_bring_ragdoll_inside_world(int32, bool)
//.text:00B6B160 ; bool __cdecl biped_bring_ragdoll_inside_world(int32, bool, const real_point3d*, real32)
//.text:00B6B260 ; bool __cdecl biped_bring_ragdoll_inside_world_recursive(int32, bool, const real_point3d*, real32, const c_runtime_node_to_havok_component_rigid_body_index_mapping*, uns32*, int32, bool)
//.text:00B6B3A0 ; bool __cdecl biped_bring_rigid_body_to_safe_position(int32, bool, const real_point3d*, real32, int32, int32, int32, bool)

void __cdecl biped_build_2d_camera_frame(const real_vector3d* forward_vector, const real_vector3d* up_vector, real_vector2d* forward_axis, real_vector2d* side_axis)
{
	INVOKE(0x00B6B770, biped_build_2d_camera_frame, forward_vector, up_vector, forward_axis, side_axis);
}

void __cdecl biped_bumped_object(int32 biped_index, int32 object_index, const real_vector3d* old_velocity)
{
	//INVOKE(0x00B6B8F0, biped_bumped_object, biped_index, object_index, linear_velocity);

	biped_datum* biped = BIPED_GET(biped_index);

	if (biped->biped.bump_ticks < 0)
	{
		if (object_index == NONE)
		{
			biped->biped.bump_ticks++;
		}
		else
		{
			biped->biped.bump_ticks = -static_cast<int8>(game_seconds_to_ticks_round(0.5f));
		}
	}
	else if (object_index != NONE)
	{
		biped_datum* bump_object = (biped_datum*)object_get(object_index);
		if (bump_object)
		{
			if (TEST_BIT(_object_mask_biped, object_get_type(object_index)))
			{
				biped_datum* bumped_biped = BIPED_GET(object_index);
				if (bumped_biped->biped.physics.get_mode() == c_character_physics_component::_mode_melee)
				{
					biped->biped.flags.set(_biped_magic_deceleration_bit, true);
				}
			}

			if (biped->biped.bump_ticks >= 0)
			{
				ai_handle_bump(biped_index, object_index, old_velocity);

				if (biped->unit.player_index != NONE || recorded_animation_controlling_unit(biped_index))
				{
					if (biped->biped.bump_object_index == object_index)
					{
						if (game_ticks_to_seconds(++biped->biped.bump_ticks) > 0.1f)
						{
							if (TEST_BIT(_object_mask_unit, bump_object->object.object_identifier.get_type()))
							{
								if (cheat.bump_possession && bump_object->unit.player_index == NONE)
								{
									player_set_unit_index(biped->unit.player_index, object_index);

									if (bump_object->object.object_identifier.get_type() == _object_type_biped)
									{
										bump_object->biped.bump_ticks = -static_cast<int8>(game_seconds_to_ticks_round(0.5f));
									}
								}
							}

							biped->biped.bump_ticks = -static_cast<int8>(game_seconds_to_ticks_round(0.5f));
						}
					}
					else
					{
						biped->biped.bump_object_index = object_index;
						biped->biped.bump_ticks = 0;
					}
				}
			}
		}
	}
}

//.text:00B6BA60 ; int32 __cdecl biped_calculate_better_melee_target(int32, int32, int32)
//.text:00B6BDA0 ; bool __cdecl biped_calculate_invisible_crouched_uber_melee_z_offset(int32, real32*)
//.text:00B6BE30 ; bool __cdecl biped_calculate_melee_aiming(int32, real_vector3d*)
//.text:00B6C1E0 ; void __cdecl biped_calculate_melee_target_parameters(int32, int32, const real_vector3d*, s_biped_melee_target_parameters*)
//.text:00B6C840 ; real32 __cdecl biped_calculate_animated_jump_z_offset_per_tick()
//.text:00B6C870 ; bool __cdecl biped_can_trade_weapon_with_player(int32, int32)
//.text:00B6CAE0 ; bool __cdecl biped_compute_function_value(int32, int32, int32, real32*, bool*, bool*)

void __cdecl biped_compute_lean(int32 biped_index, real_point3d* camera_position, real_vector3d* camera_forward, real_vector3d* camera_up)
{
	//INVOKE(0x00B6CC70, biped_compute_lean, biped_index, camera_position, camera_forward, camera_up);

	ASSERT(camera_position);
	ASSERT(camera_forward);
	ASSERT(camera_up);

	biped_datum* biped = BIPED_GET(biped_index);
	real32 lean = transition_function_evaluate(_transition_function_early, fabsf(biped->biped.lean));
	lean *= (real32)(biped->biped.lean == 0.0f ? 0 : (biped->biped.lean >= 0.0f ? 1 : -1));

	real32 lean_angle = lean * global_lean_angle;
	real32 turn_angle = lean * global_lean_turn_angle;
	real32 return_angle = lean_angle * -global_lean_return_amount;

	real_point3d pivot{};
	rotate_vector_about_axis(camera_forward, camera_up, sine(turn_angle), cosine(turn_angle));
	point_from_line3d(camera_position, camera_up, -global_lean_pivot_offset, &pivot);
	rotate_vector_about_axis(camera_up, camera_forward, sine(lean_angle), cosine(lean_angle));
	point_from_line3d(camera_position, camera_up, global_lean_pivot_offset, &pivot);
	rotate_vector_about_axis(camera_up, camera_forward, sine(return_angle), cosine(return_angle));
}

//.text:00B6CE70 ; void __cdecl biped_dash(int32, int32, int32, bool)
//.text:00B6D010 ; int32 __cdecl biped_dash_time_to_target(int32)
//.text:00B6D0D0 ; void __cdecl biped_delete(int32)
//.text:00B6D0E0 ; void __cdecl biped_died(int32, bool, bool)
//.text:00B6D280 ; void __cdecl biped_dispose_from_old_structure_bsp(int32)
//.text:00B6D290 ; void __cdecl biped_affected_by_shape_phantom_this_tick(int32)
//.text:00B6D2C0 ; void __cdecl biped_enter_climbing_physics(int32, const real_point3d*)
//.text:00B6D310 ; void __cdecl biped_enter_dead_physics(int32)
//.text:00B6D370 ; void __cdecl biped_enter_default_physics(int32)
//.text:00B6D440 ; void __cdecl biped_enter_flying_physics(int32)
//.text:00B6D480 ; void __cdecl biped_enter_ground_physics(int32)
//.text:00B6D4C0 ; void __cdecl biped_enter_posture_physics(int32, const real_point3d*, const real_vector3d*, const real_matrix4x3*)
//.text:00B6D620 ; void __cdecl biped_exit_invisible_crouched_animated_jump(int32)
//.text:00B6D780 ; void __cdecl biped_exit_seat_end(int32, int32)
//.text:00B6D850 ; bool __cdecl biped_fell_to_death(int32)
//.text:00B6D890 ; void __cdecl biped_find_pathfinding_location(int32, int16*, int32*, c_sector_ref*, real_point3d*, int32*, uns32*)
//.text:00B6DAE0 ; bool __cdecl biped_find_surface(int32, int32, int32, real32, const real_point3d*, const real_vector3d*, real_point3d*, real_vector3d*, int16*, int32*, int32*, int32*, uns32*)
//.text:00B6DCB0 ; bool __cdecl biped_fix_position(int32, int32, const real_point3d*, const real_point3d*, real_point3d*, real32, real32, bool, bool)
//.text:00B6DD20 ; void __cdecl biped_fix_position_to_pill(int32, real_point3d*, real32)
//.text:00B6E040 ; void __cdecl biped_force_ground_fitting_on(int32, bool)

void __cdecl biped_get_autoaim_pill(int32 biped_index, real_point3d* base, real_vector3d* height, real32* autoaim_width)
{
	INVOKE(0x00B6E0A0, biped_get_autoaim_pill, biped_index, base, height, autoaim_width);
}

//.text:00B6E250 ; int32 __cdecl biped_get_current_weapon(int32)
//.text:00B6E2F0 ; c_character_physics_component::e_mode __cdecl biped_get_default_movement_type(int32)

real32 __cdecl biped_get_desired_camera_height(int32 biped_index)
{
	return INVOKE(0x00B6E360, biped_get_desired_camera_height, biped_index);
}

//.text:00B6E3D0 ; real32 __cdecl biped_get_gravity_scale(int32)
//.text:00B6E450 ; void __cdecl biped_get_melee_target_physics_pill(int32, real_point3d*, real32*, real32*)
//.text:00B6E510 ; bool __cdecl biped_get_movement_ground_direction(int32, real_point3d*, real_vector3d*)
//.text:00B6E780 ; c_character_physics_component::e_mode __cdecl biped_get_movement_type(int32)
//.text:00B6E7C0 ; real32 __cdecl biped_get_physics_model_node_matrix_z_offset(int32)

void __cdecl biped_get_physics_pill(int32 biped_index, real_point3d* position, real32* height, real32* radius)
{
	INVOKE(0x00B6E840, biped_get_physics_pill, biped_index, position, height, radius);
}

//.text:00B6E850 ; void __cdecl biped_get_physics_pill_from_definition(int32, real_point3d*, real32*, real32*)
//.text:00B6E950 ; real32 __cdecl biped_get_physics_pill_height(int32)

void __cdecl biped_get_sentinel_animation_node_position_and_velocity(int32 biped_index, real_point3d* position, real_vector3d* velocity)
{
	INVOKE(0x00B6E9C0, biped_get_sentinel_animation_node_position_and_velocity, biped_index, position, velocity);
}

// 0: _unit_estimate_none
// 1: _unit_estimate_head_standing
// 2: _unit_estimate_head_crouching
// 3: _unit_estimate_gun_position
// 4: _unit_estimate_aiming_standing
// 5: _unit_estimate_aiming_crouching
void __cdecl biped_get_sight_position(int32 biped_index, int16 estimate_mode, bool offset_camera, const real_point3d* estimated_body_position, const real_vector3d* a5, const real_vector3d* desired_facing_vector, const real_vector3d* desired_gun_offset, real_point3d* camera_position)
{
	INVOKE(0x00B6EB80, biped_get_sight_position, biped_index, estimate_mode, offset_camera, estimated_body_position, a5, desired_facing_vector, desired_gun_offset, camera_position);
}

//.text:00B6F430 ; bool __cdecl biped_get_velocity_from_root_movement(int32, real_vector3d*, real_vector3d*)
//.text:00B6F560 ; void __cdecl biped_handle_deleted_object(int32, int32)
//.text:00B6F5E0 ; void __cdecl biped_handle_footfall_events(int32, const s_animation_event_data*)
//.text:00B6F750 ; void __cdecl biped_impulse_vehicle_seat(int32, e_biped_seat_impulse_type)

bool __cdecl biped_in_airborne_state(int32 biped_index)
{
	return INVOKE(0x00B6F9E0, biped_in_airborne_state, biped_index);
}

//.text:00B6F9F0 ; void __cdecl biped_initialize_for_new_structure_bsp(int32)
//.text:00B6FA00 ; bool __cdecl biped_is_animation_controlled_by_equipment(int32)
//.text:00B6FA30 ; bool __cdecl biped_is_assassination_victim(int32)
//.text:00B6FAA0 ; bool __cdecl biped_is_dead_or_dying(int32)
//.text:00B6FB00 ; bool __cdecl biped_is_running_invisible_crouched_uber_melee(int32)
//.text:00B6FB70 ; bool __cdecl biped_is_stunned(int32)
//.text:00B6FB80 ; bool __cdecl biped_is_very_heavy(int32)
//.text:00B6FBC0 ; void __cdecl biped_locate_auxilary_animations(int32)
//.text:00B6FD10 ; void __cdecl biped_make_footstep(int32, e_effect_type, e_contact_point, real32)
//.text:00B70150 ; bool __cdecl biped_melee_target_parameters_in_range(const s_biped_melee_target_parameters*, bool, bool)
//.text:00B70230 ; bool __cdecl biped_new(int32, object_placement_data*, bool*)
//.text:00B70390 ; void __cdecl biped_notify_deactivated(int32)
//.text:00B70420 ; void __cdecl biped_notify_early_mover_deleted(int32)
//.text:00B70490 ; void __cdecl biped_offset_first_person_camera(int32, real_point3d*, const real_vector3d*, const real_vector3d*)
//.text:00B70720 ; void __cdecl biped_offset_following_camera_position(int32, real_point3d*)
//.text:00B707B0 ; void __cdecl biped_pill_clear_obstacle(int32, int32)
//.text:00B70A90 ; void __cdecl biped_place(int32, s_scenario_biped*)
//.text:00B70AC0 ; bool __cdecl biped_post_update(int32)
//.text:00B70AE0 ; void __cdecl biped_postprocess_node_matrices(int32, int32, real_matrix4x3*)

void __cdecl biped_ragdoll(int32 biped_index)
{
	INVOKE(0x00B70DB0, biped_ragdoll, biped_index);

	//if (biped_index != NONE)
	//{
	//	biped_datum* biped = BIPED_GET(biped_index);
	//	if (biped)
	//	{
	//		biped->object.damage_flags.set(_object_die_act_of_god_silent_bit, true);
	//		biped->unit.special_death_type = 1;
	//		object_wake(biped_index);
	//		object_set_at_rest(biped_index, false);
	//	}
	//}
}

void __cdecl biped_render_debug(int32 biped_index)
{
	//INVOKE(0x00B70DF0, biped_render_debug, biped_index);

	if (debug_objects_physics_control_node)
	{
		real_point3d position{};
		real_vector3d velocity{};
		biped_get_sentinel_animation_node_position_and_velocity(biped_index, &position, &velocity);

		real_point3d point1 = position;
		real32 tick_length = game_tick_length();
		point1.x += (tick_length * velocity.i);
		point1.y += (tick_length * velocity.j);
		point1.z += (tick_length * velocity.k);

		render_debug_sphere(true, &position, 0.1f, global_real_argb_red);
		render_debug_line(true, &position, &point1, global_real_argb_white);
	}

	if (debug_objects_biped_autoaim_pills)
	{
		real_point3d base{};
		real_vector3d height{};
		real32 autoaim_width = 0.0f;

		biped_get_autoaim_pill(biped_index, &base, &height, &autoaim_width);
		if (magnitude_squared3d(&height) <= 0.0f)
		{
			render_debug_sphere(true, &base, autoaim_width, global_real_argb_red);
		}
		else
		{
			render_debug_pill(true, &base, &height, autoaim_width, global_real_argb_red);
		}
	}

	if (debug_objects_ground_plane)
	{

	}

	if (debug_objects_movement_mode)
	{
		biped_datum* biped = BIPED_GET(biped_index);

		real_point3d base{};
		real_vector3d height{};
		real32 autoaim_width = 0.0f;

		biped_get_autoaim_pill(biped_index, &base, &height, &autoaim_width);
		point_from_line3d(&base, &height, 1.0f, &base);

		const char* mode_string = nullptr;
		switch (biped->biped.physics.get_mode())
		{
		case c_character_physics_component::_mode_ground:
		{
			mode_string = "ground";
		}
		break;
		case c_character_physics_component::_mode_flying:
		{
			mode_string = "flying";
		}
		break;
		case c_character_physics_component::_mode_dead:
		{
			mode_string = "dead";
		}
		break;
		case c_character_physics_component::_mode_posture:
		{
			mode_string = "posture";
		}
		break;
		case c_character_physics_component::_mode_climbing:
		{
			mode_string = "climbing";
		}
		break;
		case c_character_physics_component::_mode_melee:
		{
			mode_string = "melee";
		}
		break;
		default:
		{
			UNREACHABLE();
		}
		break;
		}

		render_debug_string_at_point(&base, mode_string, global_real_argb_blue);
	}

	if (debug_biped_throttle)
	{

	}

	if (debug_objects_unit_pathfinding_surface)
	{

	}

	if (debug_objects_pendulum)
	{

	}
}

//.text:00B70E00 ; void __cdecl biped_reset(int32)
//.text:00B71030 ; void __cdecl biped_reset_interpolators(int32)
//.text:00B710B0 ; void __cdecl biped_reset_pathfinding_location(int32)
//.text:00B71130 ; void __cdecl biped_save_node_matrices(int32)
//.text:00B71210 ; void __cdecl biped_sentinel_physics_translate_origin(int32, const real_vector3d*, bool)
//.text:00B712C0 ; void __cdecl biped_set_gravity_scale(int32, real32)
//.text:00B71300 ; void __cdecl biped_setup_mixing_board(c_animation_manager*)
//.text:00B71340 ; bool __cdecl biped_should_override_deactivation(int32)
//.text:00B71390 ; bool __cdecl biped_should_remain_active(int32)
//.text:00B71470 ; void __cdecl biped_spawn_death_children(int32)
//.text:00B71500 ; bool __cdecl biped_test_position(int32, const real_point3d*)

bool __cdecl biped_update(int32 biped_index)
{
	//return INVOKE(0x00B716C0, biped_update, biped_index);

	biped_datum* biped = BIPED_GET(biped_index);

	bool affected_by_soft_ceiling = biped_update_soft_ceilings(biped_index);

	if (biped->unit.actor_index != NONE)
	{
		actor_datum* actor = DATUM_GET(actor_data, actor_datum, biped->unit.actor_index);
		biped->biped.ai_combat_status = (int8)actor->state.combat_status;
	}

	bool awake = false;

	if (biped->object.havok_component_index != NONE)
	{
		biped_update_kill_volumes(biped_index);

		int32 parent_index = object_get_root_object(biped->object.parent_object_index);
		if (parent_index == NONE)
		{
			awake = biped_update_without_parent(biped_index) || biped_update_stun(biped_index);
		}
		else
		{
			awake = biped_update_with_parent(biped_index, parent_index) || biped_update_stun(biped_index);
		}

		biped_update_camera(biped_index);
	}

	biped_update_leaning(biped_index);

	if (!affected_by_soft_ceiling)
	{
		biped_update_jetpack(biped_index);
	}

	biped_update_pendulum(biped_index);
	biped_update_keyframed_rigid_bodies(biped_index);
	biped_update_root_matrix_history(biped_index);

	return awake;
}

void __cdecl biped_update_camera(int32 biped_index)
{
	//INVOKE(0x00B717B0, biped_update_camera, biped_index);

	biped_datum* biped = BIPED_GET(biped_index);
	if (biped->unit.player_index != NONE &&
		!motor_animation_resource_busy(biped_index, _motor_resource_primary_impulse) &&
		!motor_animation_resource_busy(biped_index, _motor_resource_secondary_impulse))
	{
		real_matrix4x3* root_matrix = object_get_node_matrix(biped_index, 0);

		object_marker head_marker{};
		if (!object_get_markers_by_string_id(biped_index, STRING_ID(global, fp_body_cam), &head_marker, 1))
		{
			object_get_markers_by_string_id(biped_index, STRING_ID(global, head), &head_marker, 1);
		}

		real_vector3d offset_vector{};
		vector_from_points3d(&root_matrix->origin, &head_marker.matrix.origin, &offset_vector);

		real_vector3d aiming_vector = biped->unit.aiming_vector;
		unit_clip_vector_to_aiming_screen_bounds(biped_index, &aiming_vector, false);

		real_vector2d forward_axis{}, side_axis{};
		biped_build_2d_camera_frame(&aiming_vector, &biped->object.up, &forward_axis, &side_axis);
		real32 forward = dot_product2d((real_vector2d*)&offset_vector, &forward_axis);
		real32 left = dot_product2d((real_vector2d*)&offset_vector, &side_axis);
		real32 up = offset_vector.k;

		real_vector3d relative_offset{};
		set_real_vector3d(&relative_offset, forward, left, up);

		if (biped->biped.flags.test(_biped_first_person_camera_offset_valid_bit))
		{
			interpolate_scalar(&biped->biped.first_person_camera_offset.i, relative_offset.i, 0.01f);
			interpolate_scalar(&biped->biped.first_person_camera_offset.j, relative_offset.j, 0.01f);
			interpolate_scalar(&biped->biped.first_person_camera_offset.k, relative_offset.k, 0.01f);
		}
		else
		{
			biped->biped.first_person_camera_offset = relative_offset;
			biped->biped.flags.set(_biped_first_person_camera_offset_valid_bit, true);
		}
	}

	{
		real32 camera_height_delta = biped_get_desired_camera_height(biped_index) - biped->biped.camera_height;
		real32 camera_height = fabsf(camera_height_delta);
		real32 scale = real_pin(camera_height, 0.0f, 1.0f);
		struct biped_definition* biped_definition = TAG_GET(BIPED_TAG, struct biped_definition, biped->definition_index);
		real32 height_velocity = biped_definition->biped.camera_height_velocity_function.evaluate_scalar(scale, 0.0f);
		real32 step = real_min(height_velocity * game_tick_length(), camera_height);
		int32 sign = (camera_height_delta == 0.0f) ? 0 : (camera_height_delta >= 0.0f ? 1 : -1);
		biped->biped.camera_height += (step * (real32)sign);
	}
}

//.text:00B71AD0 ; 

void __cdecl biped_update_jetpack(int32 biped_index)
{
	if (cheat.jetpack)
	{
		biped_datum* biped = BIPED_GET(biped_index);
		if (biped && biped->unit.player_index != NONE)
		{
			if (biped_in_airborne_state(biped_index))
			{
				if (TEST_BIT(biped->unit.unit_control_flags, _unit_control_jetpack_bit))
				{
					real_vector3d linear_velocity{};
					if (TEST_BIT(biped->unit.unit_control_flags, _unit_control_crouch_modifier_bit))
					{
						// freeze velocity and adjust for gravity
						linear_velocity.i = 0.0f;
						linear_velocity.j = 0.0f;
						linear_velocity.k = global_gravity_get() * game_tick_length();
					}
					else
					{
						object_get_velocities(biped_index, &linear_velocity, nullptr);

						real32 velocity_dot = dot_product3d(&biped->unit.aiming_vector, &linear_velocity);
						if (velocity_dot < 0.0f)
						{
							velocity_dot = 0.0f;
						}

						real32 clamped_velocity_dot = fminf(1.0f, fmaxf(velocity_dot * 0.0625f, 0.0f));
						if (clamped_velocity_dot >= 1.0f)
						{
							clamped_velocity_dot = 1.0f;
						}

						real32 negative_velocity_dot = -velocity_dot;
						linear_velocity.i -= (0.2f * (linear_velocity.i + (negative_velocity_dot * biped->unit.aiming_vector.i)));
						linear_velocity.j -= (0.2f * (linear_velocity.j + (negative_velocity_dot * biped->unit.aiming_vector.j)));
						linear_velocity.k -= (0.2f * (linear_velocity.k + (negative_velocity_dot * biped->unit.aiming_vector.k)));

						real32 v11 = game_tick_length() * (9.0f + ((9.0f * clamped_velocity_dot) - (18.0f * (clamped_velocity_dot * clamped_velocity_dot))));
						linear_velocity.i += v11 * biped->unit.aiming_vector.i;
						linear_velocity.j += v11 * biped->unit.aiming_vector.j;
						linear_velocity.k += v11 * biped->unit.aiming_vector.k;

						c_motor_request motor_request{};
						motor_request.setup_force_airborne(_action_none);
						motor_system_submit(biped_index, &motor_request);

						linear_velocity.k += global_gravity_get() * game_tick_length();
					}
					object_set_velocities(biped_index, &linear_velocity, nullptr);
				}
			}
		}
	}
}

void __cdecl biped_update_keyframed_rigid_bodies(int32 biped_index)
{
	INVOKE(0x00B71AE0, biped_update_keyframed_rigid_bodies, biped_index);
}

void __cdecl biped_update_kill_volumes(int32 biped_index)
{
	INVOKE(0x00B71C10, biped_update_kill_volumes, biped_index);
}

void __cdecl biped_update_leaning(int32 biped_index)
{
	//INVOKE(0x00B71D10, biped_update_leaning, biped_index);

	biped_datum* biped = BIPED_GET(biped_index);
	if (biped && biped->unit.player_index != NONE && biped->biped.lean != 0.0f)
	{
		interpolate_scalar(&biped->biped.lean, 0.0f, 0.035f);

		if (IN_RANGE_INCLUSIVE(biped->biped.lean, -0.035f, 0.035f))
		{
			biped->biped.lean = 0.0f;
		}
	}
}

//.text:00B71D20 ; bool __cdecl biped_update_melee_turning(int32, s_animation_control_data*)
//.text:00B71DF0 ; void __cdecl biped_update_mixing_board(int32)
//.text:00B71E90 ; void __cdecl biped_update_moving_turn_constraints(int32)

void __cdecl biped_update_pendulum(int32 biped_index)
{
	INVOKE(0x00B72280, biped_update_pendulum, biped_index);
}

void __cdecl biped_update_root_matrix_history(int32 biped_index)
{
	INVOKE(0x00B72580, biped_update_root_matrix_history, biped_index);
}

bool __cdecl biped_update_soft_ceilings(int32 biped_index)
{
	return INVOKE(0x00B72610, biped_update_soft_ceilings, biped_index);
}

bool __cdecl biped_update_stun(int32 biped_index)
{
	return INVOKE(0x00B726A0, biped_update_stun, biped_index);
}

bool __cdecl biped_update_with_parent(int32 biped_index, int32 parent_index)
{
	return INVOKE(0x00B726F0, biped_update_with_parent, biped_index, parent_index);
}

bool __cdecl biped_update_without_parent(int32 biped_index)
{
	return INVOKE(0x00B728F0, biped_update_without_parent, biped_index);
}

//.text:00B72B20 ; bool __cdecl biped_verify_surface_reference(int16, int32, int32, int32, int32)
//.text:00B72BD0 ; void __cdecl bipeds_dispose()
//.text:00B72BE0 ; void __cdecl bipeds_dispose_from_old_map()
//.text:00B72BF0 ; void __cdecl bipeds_initialize()
//.text:00B72C00 ; void __cdecl bipeds_initialize_for_new_map()
//.text:00B72C10 ; 
//.text:00B72C20 ; 
//.text:00B72C30 ; 
//.text:00B72C50 ; 
//.text:00B72C70 ; 
//.text:00B72C80 ; 
//.text:00B72CB0 ; void __cdecl process_all_ik_points(int32, int32, int32, bool, real32, const c_animation_manager*, ik_point_iterator*, int32, real_matrix4x3*)
//.text:00B72EC0 ; void __cdecl process_mode_ik(int32, int32, int32, real_matrix4x3*)
//.text:00B72FD0 ; void __cdecl process_weapon_ik(int32, int32, int32, real_matrix4x3*)
//.text:00B730E0 ; 
//.text:00B73110 ; 
//.text:00B73130 ; 
//.text:00B73150 ; 
//.text:00B73180 ; 

