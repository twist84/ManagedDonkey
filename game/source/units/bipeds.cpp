#include "units/bipeds.hpp"

#include "ai/ai.hpp"
#include "cutscene/recorded_animations.hpp"
#include "game/cheats.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "physics/character_physics.hpp"
#include "render/render_debug.hpp"
#include "units/bipeds.hpp"

HOOK_DECLARE(0x00B6B8F0, biped_bumped_object);
HOOK_DECLARE(0x00B70DF0, biped_render_debug);

bool debug_objects_physics_control_node = false;
bool debug_objects_biped_autoaim_pills = false;
bool debug_objects_ground_plane = false;
bool debug_objects_movement_mode = false;
bool debug_biped_throttle = false;
bool debug_objects_unit_pathfinding_surface = false;
bool debug_objects_pendulum = false;

void __cdecl biped_bumped_object(long biped_index, long object_index, vector3d const* old_velocity)
{
	//INVOKE(0x00B6B8F0, biped_bumped_object, biped_index, object_index, linear_velocity);

	biped_datum* biped = (biped_datum*)object_get_and_verify_type(biped_index, _object_mask_biped);

	if (biped->biped.bump_ticks < 0)
	{
		if (object_index == NONE)
		{
			biped->biped.bump_ticks++;
			return;
		}

		biped->biped.bump_ticks = -static_cast<char>(game_seconds_to_ticks_round(0.5f));
	}

	if (object_index == NONE)
		return;

	biped_datum* bump_object = (biped_datum*)object_get(object_index);
	if (!bump_object)
		return;

	if (TEST_BIT(_object_mask_biped, object_get_type(object_index)))
	{
		biped_datum* bumped_biped = (biped_datum*)object_get_and_verify_type(object_index, _object_mask_biped);
		if (bumped_biped->biped.physics.get_mode() == c_character_physics_component::_mode_melee)
		{
			//biped->biped.flags.set(15, true);
			SET_BIT(biped->biped.flags, 15, true);
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
								bump_object->biped.bump_ticks = -static_cast<char>(game_seconds_to_ticks_round(0.5f));
						}
					}

					biped->biped.bump_ticks = -static_cast<char>(game_seconds_to_ticks_round(0.5f));
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

void __cdecl biped_get_autoaim_pill(long biped_index, real_point3d* base, vector3d* height, real* autoaim_width)
{
	INVOKE(0x00B6E0A0, biped_get_autoaim_pill, biped_index, base, height, autoaim_width);
}

void __cdecl biped_get_physics_pill(long biped_index, real_point3d* position, real* height, real* radius)
{
	INVOKE(0x00B6E840, biped_get_physics_pill, biped_index, position, height, radius);
}

void __cdecl biped_get_sentinel_animation_node_position_and_velocity(long biped_index, real_point3d* position, vector3d* velocity)
{
	INVOKE(0x00B6E9C0, biped_get_sentinel_animation_node_position_and_velocity, biped_index, position, velocity);
}

// e_unit_estimate_mode?
// e_unit_estimate_position_mode?
// 0: _unit_estimate_none
// 3: _unit_estimate_gun_position
// 6: k_unit_estimate_position_modes_count
//
// 0   crouch = biped_is_running_invisible_crouched_uber_melee(biped_index) ? 0.0f : biped->unit.crouch;
// 1,4 crouch = 0.0f;
// 2,5 crouch = 1.0f;
void __cdecl biped_get_sight_position(long biped_index, short estimate_mode, bool offset_camera, real_point3d const* estimated_body_position, vector3d const* a5, vector3d const* desired_facing_vector, vector3d const* desired_gun_offset, real_point3d* camera_position)
{
	INVOKE(0x00B6EB80, biped_get_sight_position, biped_index, estimate_mode, offset_camera, estimated_body_position, a5, desired_facing_vector, desired_gun_offset, camera_position);
}

void __cdecl biped_scripting_ragdoll(long biped_index)
{
	INVOKE(0x00B70DB0, biped_scripting_ragdoll, biped_index);
}

void __cdecl biped_render_debug(long biped_index)
{
	//INVOKE(0x00B70DF0, biped_render_debug, biped_index);

	if (debug_objects_physics_control_node)
	{
		real_point3d position{};
		vector3d velocity{};
		biped_get_sentinel_animation_node_position_and_velocity(biped_index, &position, &velocity);

		real_point3d point1 = position;
		real tick_length = game_tick_length();
		point1.x += (tick_length * velocity.i);
		point1.y += (tick_length * velocity.j);
		point1.z += (tick_length * velocity.k);

		render_debug_sphere(true, &position, 0.1f, global_real_argb_red);
		render_debug_line(true, &position, &point1, global_real_argb_white);
	}

	if (debug_objects_biped_autoaim_pills)
	{
		real_point3d base{};
		vector3d height{};
		real autoaim_width = 0.0f;

		biped_get_autoaim_pill(biped_index, &base, &height, &autoaim_width);
		if (magnitude_squared3d(&height) <= 0.0f)
			render_debug_sphere(true, &base, autoaim_width, global_real_argb_red);
		else
			render_debug_pill(true, &base, &height, autoaim_width, global_real_argb_red);
	}

	if (debug_objects_ground_plane)
	{

	}

	if (debug_objects_movement_mode)
	{
		biped_datum* biped = (biped_datum*)object_get_and_verify_type(biped_index, _object_mask_biped);

		real_point3d base{};
		vector3d height{};
		real autoaim_width = 0.0f;

		biped_get_autoaim_pill(biped_index, &base, &height, &autoaim_width);
		point_from_line3d(&base, &height, 1.0f, &base);

		char const* mode_string = NULL;
		switch (biped->biped.physics.get_mode())
		{
		case c_character_physics_component::_mode_ground:
			mode_string = "ground";
			break;
		case c_character_physics_component::_mode_flying:
			mode_string = "flying";
			break;
		case c_character_physics_component::_mode_dead:
			mode_string = "dead";
			break;
		case c_character_physics_component::_mode_posture:
			mode_string = "posture";
			break;
		case c_character_physics_component::_mode_climbing:
			mode_string = "climbing";
			break;
		case c_character_physics_component::_mode_melee:
			mode_string = "melee";
			break;
		default:
			ASSERT2("unreachable");
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

