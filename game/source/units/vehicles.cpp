#include "units/vehicles.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "objects/objects.hpp"
#include "render/render_debug.hpp"
#include "units/vehicle_type_component.hpp"

#include <math.h>

HOOK_DECLARE(0x00B76AB0, vehicle_render_debug);

//.text:00B75420 ; public: __cdecl suspension_animation_iterator::suspension_animation_iterator()
//.text:00B75440 ; 
//.text:00B75450 ; 
//.text:00B75460 ; 
//.text:00B75470 ; 
//.text:00B75480 ; bool __cdecl object_is_huge_vehicle(long)
//.text:00B754F0 ; 

bool __cdecl vehicle_about_to_detonate_near_any_player(long* out_vehicle_index)
{
	return INVOKE(0x00B75510, vehicle_about_to_detonate_near_any_player, out_vehicle_index);
}

//.text:00B756E0 ; void __cdecl vehicle_adjust_gravity(long, real_vector3d*)
//.text:00B75770 ; bool __cdecl vehicle_auto_control(long)
//.text:00B757B0 ; 
//.text:00B757C0 ; bool __cdecl vehicle_calculate_aabb_phantom_bounds(long, real_rectangle3d*)
//.text:00B75810 ; bool __cdecl vehicle_can_be_teleported(long)
//.text:00B75860 ; bool __cdecl vehicle_causes_collision_damage(long)
//.text:00B758B0 ; bool __cdecl vehicle_compute_function_value(long, long, long, real*, bool*, bool*)
//.text:00B75A90 ; long __cdecl vehicle_count_bipeds_killed(long)
//.text:00B75AD0 ; void __cdecl vehicle_deplete_function_variables(long)
//.text:00B75B10 ; void __cdecl vehicle_enable_ghost_effects(bool)
//.text:00B75B20 ; void __cdecl vehicle_find_pathfinding_location(long, short*, long*, c_sector_ref*, real_point3d*, long*, dword*)
//.text:00B75D90 ; bool __cdecl vehicle_get_auto_turret_damage_owner(long, s_damage_owner*)
//.text:00B75DD0 ; short __cdecl vehicle_get_driver_seat(long, long*)

void __cdecl vehicle_get_seat_position(long vehicle_index, short seat_index, real_point3d* seat_position)
{
	INVOKE(0x00B75E50, vehicle_get_seat_position, vehicle_index, seat_index, seat_position);
}

e_vehicle_type __cdecl vehicle_get_type(long vehicle_index)
{
	return INVOKE(0x00B75EE0, vehicle_get_type, vehicle_index);
}

//.text:00B75F20 ; long __cdecl vehicle_get_ultimate_parent(long)
//.text:00B75F70 ; void __cdecl vehicle_handle_deleted_object(long, long)
//.text:00B75FB0 ; void __cdecl vehicle_handle_tag_change(long)
//.text:00B75FF0 ; bool __cdecl vehicle_has_fixed_camera_yaw(long)
//.text:00B76050 ; vehicle_has_gameplay_modifier?
//.text:00B76100 ; void __cdecl vehicle_hover(long, bool)
//.text:00B761C0 ; void __cdecl vehicle_impulse_channel_node_preprocess_callback(long, render_model_definition const*, c_animation_channel*, real, real, real, void*)
//.text:00B76230 ; bool __cdecl vehicle_is_flipped(long)
//.text:00B76270 ; bool __cdecl vehicle_is_running_trick(long)
//.text:00B762B0 ; bool __cdecl vehicle_is_stopped(long)

bool __cdecl vehicle_moving_near_any_player(long* out_vehicle_index)
{
	return INVOKE(0x00B762F0, vehicle_moving_near_any_player, out_vehicle_index);
}

//.text:00B764D0 ; bool __cdecl vehicle_new(long, object_placement_data*, bool*)
//.text:00B765E0 ; 
//.text:00B765F0 ; bool __cdecl vehicle_physics_disabled(long)
//.text:00B76630 ; void __cdecl vehicle_place(long, s_scenario_vehicle*)
//.text:00B766F0 ; void __cdecl vehicle_preprocess_node_orientations(long, c_static_flags<255> const*, long, real_orientation*)
//.text:00B76720 ; void __cdecl vehicle_process_animation_channels(long, void(__cdecl*)(long, render_model_definition const*, c_animation_channel*, real, real, real, void*), void*)

void __cdecl vehicle_render_debug(long vehicle_index)
{
	vehicle_datum* vehicle = vehicle_get(vehicle_index);
	struct vehicle_definition* vehicle_definition = (struct vehicle_definition*)tag_get(VEHICLE_TAG, vehicle->definition_index);

	if (debug_objects_vehicle_physics && vehicle->object.parent_object_index == NONE)
	{
		if (vehicle_definition->vehicle.physics.flags.test(_havok_vehicle_physics_definition_flags_invalid))
		{
			real_point3d origin{};
			object_get_origin(vehicle_index, &origin);
			render_debug_string_at_point(&origin, "invalid vehicle!!! read your debug.txt", global_real_argb_red);

			// donkey note: there is debug.txt yet ;)
		}

		s_vehicle_engine_definition const* engine_definition = NULL;
		s_vehicle_engine const* engine = vehicle->vehicle.type_component.get_engine(vehicle_index);
		switch (vehicle_get_type(vehicle_index))
		{
		case _vehicle_type_human_tank:
			engine_definition = &vehicle_get_human_tank_definition(vehicle->definition_index)->engine;
			break;
		case _vehicle_type_human_jeep:
			engine_definition = &vehicle_get_human_tank_definition(vehicle->definition_index)->engine;
			break;
		case _vehicle_type_chopper:
			engine_definition = &vehicle_get_chopper_definition(vehicle->definition_index)->engine;
			break;
		}
		ASSERT((engine == NULL) == (engine_definition == NULL));

		if (engine && engine_definition)
		{
			if (engine_definition->engine_max_angular_velocity > k_test_real_epsilon)
			{
				real v0 = ((vehicle_engine_get_rpm_function_scale(engine) * engine->engine_angular_velocity) / engine_definition->engine_max_angular_velocity) * 20.0f;
				long v1 = static_cast<long>(fminf(fmaxf(v0, 0.0f), 20.0f));
				long v2 = static_cast<long>(fminf(fmaxf(v0 - 20.0f, 0.0f), 20.0f));
				long v3 = 20 - v1;
				long v4 = 20 - v2;

				char string[1024]{};
				csnzprintf(string, 1024, "gear %d/%d **%ld---%ld^^^%ld", engine->gear, engine_definition->gears.count - 1, v3, v1, v4);

				real_point3d origin{};
				object_get_origin(vehicle_index, &origin);
				origin.z += 1.0f;
				render_debug_string_at_point(&origin, string, v2 ? global_real_argb_red : global_real_argb_aqua);
			}
		}
	}
}

//.text:00B76AC0 ; void __cdecl vehicle_reset(long)
//.text:00B76C80 ; void __cdecl vehicle_set_auto_turret(long, long, real, real, real)
//.text:00B76CC0 ; void __cdecl vehicle_set_auto_turret(long, long, real, real, real, long)
//.text:00B76D50 ; void __cdecl vehicle_set_auto_turret_damage_owner(long, long)
//.text:00B76D90 ; bool __cdecl vehicle_should_override_deactivation(long)
//.text:00B76DD0 ; bool __cdecl vehicle_start_trick(long, e_vehicle_trick_type)
//.text:00B76E10 ; long __cdecl vehicle_player_occupant(long, long*, bool, bool)
//.text:00B76EC0 ; bool __cdecl vehicle_update(long)
//.text:00B770A0 ; void __cdecl vehicles_dispose()
//.text:00B770B0 ; void __cdecl vehicles_dispose_from_old_map()
//.text:00B770C0 ; void __cdecl vehicles_initialize()
//.text:00B770D0 ; void __cdecl vehicles_initialize_for_new_map()

vehicle_datum* vehicle_get(long vehicle_index)
{
	return (vehicle_datum*)object_get_and_verify_type(vehicle_index, _object_mask_vehicle);
}

