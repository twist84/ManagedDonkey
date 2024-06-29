#include "units/vehicles.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "objects/objects.hpp"
#include "render/render_debug.hpp"
#include "units/vehicle_type_component.hpp"

#include <math.h>

HOOK_DECLARE(0x00B76AB0, vehicle_render_debug);

bool __cdecl vehicle_about_to_detonate_near_any_player(long* out_vehicle_index)
{
	return INVOKE(0x00B75510, vehicle_about_to_detonate_near_any_player, out_vehicle_index);
}

void __cdecl vehicle_get_seat_position(long vehicle_index, short seat_index, real_point3d* seat_position)
{
	INVOKE(0x00B75E50, vehicle_get_seat_position, vehicle_index, seat_index, seat_position);
}

e_vehicle_type __cdecl vehicle_get_type(long vehicle_index)
{
	return INVOKE(0x00B75EE0, vehicle_get_type, vehicle_index);
}

bool __cdecl vehicle_moving_near_any_player(long* out_vehicle_index)
{
	return INVOKE(0x00B762F0, vehicle_moving_near_any_player, out_vehicle_index);
}

void __cdecl vehicle_render_debug(long vehicle_index)
{
	vehicle_datum* vehicle = (vehicle_datum*)object_get_and_verify_type(vehicle_index, _object_mask_vehicle);
	_vehicle_definition* vehicle_definition = (_vehicle_definition*)tag_get(VEHICLE_TAG, vehicle->object.definition_index);

	if (debug_objects_vehicle_physics && vehicle->object.parent_object_index == NONE)
	{
		s_vehicle_engine_definition* engine_definition = NULL;
		s_vehicle_engine* const engine = vehicle->vehicle.type_component.get_engine(vehicle_index);
		switch (vehicle_get_type(vehicle_index))
		{
		case _vehicle_type_human_tank:
			engine_definition = &vehicle_definition->physics_types.type_human_tank[0].engine;
			break;
		case _vehicle_type_human_jeep:
			engine_definition = &vehicle_definition->physics_types.type_human_jeep[0].engine;
			break;
		case _vehicle_type_chopper:
			engine_definition = &vehicle_definition->physics_types.type_chopper[0].engine;
			break;
		}
		ASSERT((engine == NULL) == (engine_definition == NULL));

		if (engine && engine_definition->engine_max_angular_velocity > k_test_real_epsilon)
		{
			real v0 = ((vehicle_engine_get_rpm_function_scale(engine) * engine->__unknown4) / engine_definition->engine_max_angular_velocity) * 20.0f;
			long v1 = static_cast<long>(fminf(fmaxf(v0, 0.0f), 20.0f));
			long v2 = static_cast<long>(fminf(fmaxf(v0 - 20.0f, 0.0f), 20.0f));
			long v3 = 20 - v1;
			long v4 = 20 - v2;

			char string[1024]{};
			csnzprintf(string, 1024, "gear %d/%d **%ld---%ld^^^%ld", engine->gear, engine_definition->gears.count() - 1, v3, v1, v4);

			real_point3d origin{};
			object_get_origin(vehicle_index, &origin);
			origin.z += 1.0f;
			render_debug_string_at_point(&origin, string, v2 ? global_real_argb_red : global_real_argb_aqua);
		}
	}
}

