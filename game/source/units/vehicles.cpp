#include "units/vehicles.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "objects/objects.hpp"
#include "render/render_debug.hpp"
#include "units/vehicle_type_chopper.hpp"
#include "units/vehicle_type_component.hpp"

#include <math.h>

HOOK_DECLARE(0x00B76AB0, vehicle_render_debug);

//.text:00B75420 ; public: __cdecl suspension_animation_iterator::suspension_animation_iterator()
//.text:00B75440 ; 
//.text:00B75450 ; 
//.text:00B75460 ; 
//.text:00B75470 ; 
//.text:00B75480 ; bool __cdecl object_is_huge_vehicle(int32)
//.text:00B754F0 ; 

bool __cdecl vehicle_about_to_detonate_near_any_player(int32* out_vehicle_index)
{
	return INVOKE(0x00B75510, vehicle_about_to_detonate_near_any_player, out_vehicle_index);
}

//.text:00B756E0 ; void __cdecl vehicle_adjust_gravity(int32, real_vector3d*)
//.text:00B75770 ; bool __cdecl vehicle_auto_control(int32)
//.text:00B757B0 ; bool __cdecl vehicle_build_update(int32, void*, int16*, int16)
//.text:00B757C0 ; bool __cdecl vehicle_calculate_aabb_phantom_bounds(int32, real_rectangle3d*)
//.text:00B75810 ; bool __cdecl vehicle_can_be_teleported(int32)
//.text:00B75860 ; bool __cdecl vehicle_causes_collision_damage(int32)
//.text:00B758B0 ; bool __cdecl vehicle_compute_function_value(int32, int32, int32, real32*, bool*, bool*)
//.text:00B75A90 ; int32 __cdecl vehicle_count_bipeds_killed(int32)
//.text:00B75AD0 ; void __cdecl vehicle_deplete_function_variables(int32)
//.text:00B75B10 ; void __cdecl vehicle_enable_ghost_effects(bool)
//.text:00B75B20 ; void __cdecl vehicle_find_pathfinding_location(int32, int16*, int32*, c_sector_ref*, real_point3d*, int32*, uns32*)
//.text:00B75D90 ; bool __cdecl vehicle_get_auto_turret_damage_owner(int32, s_damage_owner*)
//.text:00B75DD0 ; int16 __cdecl vehicle_get_driver_seat(int32, int32*)

void __cdecl vehicle_get_seat_position(int32 vehicle_index, int16 seat_index, real_point3d* seat_position)
{
	INVOKE(0x00B75E50, vehicle_get_seat_position, vehicle_index, seat_index, seat_position);
}

e_vehicle_type __cdecl vehicle_get_type(int32 vehicle_index)
{
	return INVOKE(0x00B75EE0, vehicle_get_type, vehicle_index);
}

//.text:00B75F20 ; int32 __cdecl vehicle_get_ultimate_parent(int32)
//.text:00B75F70 ; void __cdecl vehicle_handle_deleted_object(int32, int32)
//.text:00B75FB0 ; void __cdecl vehicle_handle_tag_change(int32)
//.text:00B75FF0 ; bool __cdecl vehicle_has_fixed_camera_yaw(int32)
//.text:00B76050 ; bool __cdecl vehicle_has_gameplay_modifier(int32, e_gameplay_modifier, int32*)
//.text:00B76100 ; void __cdecl vehicle_hover(int32, bool)
//.text:00B761C0 ; void __cdecl vehicle_impulse_channel_node_preprocess_callback(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*)
//.text:00B76230 ; bool __cdecl vehicle_is_flipped(int32)
//.text:00B76270 ; bool __cdecl vehicle_is_running_trick(int32)
//.text:00B762B0 ; bool __cdecl vehicle_is_stopped(int32)

bool __cdecl vehicle_moving_near_any_player(int32* out_vehicle_index)
{
	return INVOKE(0x00B762F0, vehicle_moving_near_any_player, out_vehicle_index);
}

//.text:00B764D0 ; bool __cdecl vehicle_new(int32, object_placement_data*, bool*)
//.text:00B765E0 ; bool __cdecl vehicle_parse_update(int32, void*, int16*, int16)
//.text:00B765F0 ; bool __cdecl vehicle_physics_disabled(int32)
//.text:00B76630 ; void __cdecl vehicle_place(int32, s_scenario_vehicle*)
//.text:00B766F0 ; void __cdecl vehicle_preprocess_node_orientations(int32, const c_static_flags<255>*, int32, real_orientation*)
//.text:00B76720 ; void __cdecl vehicle_process_animation_channels(int32, void(__cdecl*)(int32, const render_model_definition*, c_animation_channel*, real32, real32, real32, void*), void*)

void __cdecl vehicle_render_debug(int32 vehicle_index)
{
	vehicle_datum* vehicle = VEHICLE_GET(vehicle_index);
	struct vehicle_definition* vehicle_definition = TAG_GET(VEHICLE_TAG, struct vehicle_definition, vehicle->definition_index);

	if (debug_objects_vehicle_physics && vehicle->object.parent_object_index == NONE)
	{
		if (vehicle_definition->vehicle.physics.flags.test(_havok_vehicle_physics_definition_flags_invalid))
		{
			real_point3d text_point{};
			object_get_origin(vehicle_index, &text_point);
			render_debug_string_at_point(&text_point, "invalid vehicle!!! read your debug.txt", global_real_argb_red);
		}

		const s_vehicle_engine_definition* engine_definition = NULL;
		const s_vehicle_engine* engine = vehicle->vehicle.type_component.get_engine(vehicle_index);
		switch (vehicle_get_type(vehicle_index))
		{
		case _vehicle_type_human_tank:
		{
			engine_definition = &vehicle_get_human_tank_definition(vehicle->definition_index)->engine;
		}
		break;
		case _vehicle_type_human_jeep:
		{
			engine_definition = &vehicle_get_human_jeep_definition(vehicle->definition_index)->engine;
		}
		break;
		case _vehicle_type_chopper:
		{
			engine_definition = &vehicle_get_chopper_definition(vehicle->definition_index)->engine;

			real_point3d magic_origin{};
			const real_vector3d* magic_turning_vector = c_vehicle_type_chopper::get_debug_magic_force_vector(vehicle_index, &magic_origin);
			if (magic_turning_vector)
			{
				render_debug_point(true, &magic_origin, 0.05f, global_real_argb_blue);
				render_debug_vector(true, &magic_origin, magic_turning_vector, 2.0f, global_real_argb_red);
			}
		}
		break;
		}
		ASSERT((engine == NULL) == (engine_definition == NULL));

		if (engine && engine_definition)
		{
			if (engine_definition->engine_max_angular_velocity > k_test_real_epsilon)
			{
				//const int32 ticks_over;
				//const int32 ticks;
				//const int32 max_ticks;
				// ^ these are some of the following `int32` values

				const real32 engine_speed = ((vehicle_engine_get_rpm_function_scale(engine) * engine->engine_angular_velocity) / engine_definition->engine_max_angular_velocity) * 20.0f;
				int32 v1 = (int32)fminf(fmaxf(engine_speed, 0.0f), 20.0f);
				int32 v2 = (int32)fminf(fmaxf(engine_speed - 20.0f, 0.0f), 20.0f);
				int32 v3 = 20 - v1;
				int32 v4 = 20 - v2;

				char text_buffer[1024]{};
				csnzprintf(text_buffer, NUMBEROF(text_buffer), "gear %d/%d **%ld---%ld^^^%ld",
					engine->gear,
					engine_definition->gears.count - 1,
					v3,
					v1,
					v4);

				real_point3d text_point{};
				object_get_origin(vehicle_index, &text_point);
				static real32 heigh_offset = 1.0f;
				text_point.z += heigh_offset;

				const real_argb_color* color = v2 ? global_real_argb_red : global_real_argb_aqua;
				render_debug_string_at_point(&text_point, text_buffer, color);
			}
		}
	}
}

//.text:00B76AC0 ; void __cdecl vehicle_reset(int32)
//.text:00B76C80 ; void __cdecl vehicle_set_auto_turret(int32, int32, real32, real32, real32)
//.text:00B76CC0 ; void __cdecl vehicle_set_auto_turret(int32, int32, real32, real32, real32, int32)
//.text:00B76D50 ; void __cdecl vehicle_set_auto_turret_damage_owner(int32, int32)
//.text:00B76D90 ; bool __cdecl vehicle_should_override_deactivation(int32)
//.text:00B76DD0 ; bool __cdecl vehicle_start_trick(int32, e_vehicle_trick_type)
//.text:00B76E10 ; int32 __cdecl vehicle_player_occupant(int32, int32*, bool, bool)
//.text:00B76EC0 ; bool __cdecl vehicle_update(int32)
//.text:00B770A0 ; void __cdecl vehicles_dispose()
//.text:00B770B0 ; void __cdecl vehicles_dispose_from_old_map()
//.text:00B770C0 ; void __cdecl vehicles_initialize()
//.text:00B770D0 ; void __cdecl vehicles_initialize_for_new_map()

