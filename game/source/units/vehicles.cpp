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
	byte* vehicle = static_cast<byte*>(object_get_and_verify_type(vehicle_index, _object_mask_vehicle));
	REFERENCE_DECLARE(vehicle, long, object_definition_index);
	REFERENCE_DECLARE(vehicle + 0x14, long, parent_object_index);
	REFERENCE_DECLARE(vehicle + 0x5A4, c_vehicle_type_component, type_component);

	_vehicle_definition* vehicle_definition = static_cast<_vehicle_definition*>(tag_get(VEHICLE_TAG, object_definition_index));

	if (debug_objects_vehicle_physics && parent_object_index == NONE)
	{
		s_vehicle_engine_definition* engine_defintiion = NULL;
		s_vehicle_engine* const engine = type_component.get_engine(vehicle_index);
		switch (vehicle_get_type(vehicle_index))
		{
		case _vehicle_type_human_tank:
			engine_defintiion = &vehicle_definition->physics_types.type_human_tank[0].engine;
			break;
		case _vehicle_type_human_jeep:
			engine_defintiion = &vehicle_definition->physics_types.type_human_jeep[0].engine;
			break;
		case _vehicle_type_chopper:
			engine_defintiion = &vehicle_definition->physics_types.type_chopper[0].engine;
			break;
		}
		ASSERT((engine == NULL) == (engine_defintiion == NULL));

		if (engine && engine_defintiion->engine_max_angular_velocity > 0.001f)
		{
			real v11 = ((vehicle_engine_get_rpm_function_scale(engine) * engine->__unknown4) / engine_defintiion->engine_max_angular_velocity) * 20.0f;

			real v12 = fmaxf(v11, 0.0f);
			if (v12 >= 20.0f)
				v12 = 20.0f;

			real v13 = fmaxf(v11 - 20.0f, 0.0f);
			if (v13 >= 20.0f)
				v13 = 20.0f;

			long v16 = 20 - static_cast<long>(v12);
			long v17 = static_cast<long>(v12);
			long v18 = 20 - static_cast<long>(v13);

			char string[1024]{};
			csnzprintf(string, 1024, "gear %d/%d **%ld---%ld^^^%ld", engine->current_gear, engine_defintiion->gears.count() - 1, v16, v17, v18);

			real_argb_color const* v14 = global_real_argb_red;
			if (!static_cast<long>(v13))
				v14 = global_real_argb_aqua;

			real_point3d origin{};
			object_get_origin(vehicle_index, &origin);
			origin.z += 1.0f;
			render_debug_string_at_point(&origin, string, v14);
		}
	}
}

