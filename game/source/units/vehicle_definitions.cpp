#include "units/vehicle_definitions.hpp"

#include "cache/cache_files.hpp"

void __cdecl vehicle_definition_fixup(int32 vehicle_definition_index)
{
	INVOKE(0x00B95510, vehicle_definition_fixup, vehicle_definition_index);
}

e_vehicle_type __cdecl vehicle_definition_get_default_type(int32 vehicle_definition_index)
{
	return INVOKE(0x00B95540, vehicle_definition_get_default_type, vehicle_definition_index);
}

void __cdecl vehicle_definition_teardown(int32 vehicle_definition_index)
{
	INVOKE(0x00B95580, vehicle_definition_teardown, vehicle_definition_index);
}

const s_vehicle_alien_fighter_definition* __cdecl vehicle_get_alien_fighter_definition(int32 vehicle_definition_index)
{
	//return INVOKE(0x00B955B0, vehicle_get_alien_fighter_definition, vehicle_definition_index);

	const s_vehicle_definition* vehicle_definition = TAG_GET(VEHICLE_TAG, const s_vehicle_definition, vehicle_definition_index);
	const s_vehicle_alien_fighter_definition* alien_fighter_definition = (const s_vehicle_alien_fighter_definition*)vehicle_definition->vehicle.physics_types.tag_blocks[_vehicle_type_alien_fighter].address;
	return alien_fighter_definition;
}

const s_vehicle_alien_scout_definition* __cdecl vehicle_get_alien_scout_definition(int32 vehicle_definition_index)
{
	//return INVOKE(0x00B955D0, vehicle_get_alien_scout_definition, vehicle_definition_index);

	const s_vehicle_definition* vehicle_definition = TAG_GET(VEHICLE_TAG, const s_vehicle_definition, vehicle_definition_index);
	const s_vehicle_alien_scout_definition* alien_scout_definition = (const s_vehicle_alien_scout_definition*)vehicle_definition->vehicle.physics_types.tag_blocks[_vehicle_type_alien_scout].address;
	return alien_scout_definition;
}

const s_vehicle_chopper_definition* __cdecl vehicle_get_chopper_definition(int32 vehicle_definition_index)
{
	//return INVOKE(0x00B955F0, vehicle_get_chopper_definition, vehicle_definition_index);

	const s_vehicle_definition* vehicle_definition = TAG_GET(VEHICLE_TAG, const s_vehicle_definition, vehicle_definition_index);
	const s_vehicle_chopper_definition* chopper_definition = (const s_vehicle_chopper_definition*)vehicle_definition->vehicle.physics_types.tag_blocks[_vehicle_type_chopper].address;
	return chopper_definition;
}

const s_vehicle_guardian_definition* __cdecl vehicle_get_guardian_definition(int32 vehicle_definition_index)
{
	//return INVOKE(0x00B95610, vehicle_get_guardian_definition, vehicle_definition_index);

	const s_vehicle_definition* vehicle_definition = TAG_GET(VEHICLE_TAG, const s_vehicle_definition, vehicle_definition_index);
	const s_vehicle_guardian_definition* guardian_definition = (const s_vehicle_guardian_definition*)vehicle_definition->vehicle.physics_types.tag_blocks[_vehicle_type_guardian].address;
	return guardian_definition;
}

const s_vehicle_human_jeep_definition* __cdecl vehicle_get_human_jeep_definition(int32 vehicle_definition_index)
{
	//return INVOKE(0x00B95630, vehicle_get_human_jeep_definition, vehicle_definition_index);

	const s_vehicle_definition* vehicle_definition = TAG_GET(VEHICLE_TAG, const s_vehicle_definition, vehicle_definition_index);
	const s_vehicle_human_jeep_definition* human_jeep_definition = (const s_vehicle_human_jeep_definition*)vehicle_definition->vehicle.physics_types.tag_blocks[_vehicle_type_human_jeep].address;
	return human_jeep_definition;
}

const s_vehicle_human_plane_definition* __cdecl vehicle_get_human_plane_definition(int32 vehicle_definition_index)
{
	//return INVOKE(0x00B95650, vehicle_get_human_plane_definition, vehicle_definition_index);

	const s_vehicle_definition* vehicle_definition = TAG_GET(VEHICLE_TAG, const s_vehicle_definition, vehicle_definition_index);
	const s_vehicle_human_plane_definition* human_plane_definition = (const s_vehicle_human_plane_definition*)vehicle_definition->vehicle.physics_types.tag_blocks[_vehicle_type_human_plane].address;
	return human_plane_definition;
}

const s_vehicle_human_tank_definition* __cdecl vehicle_get_human_tank_definition(int32 vehicle_definition_index)
{
	//return INVOKE(0x00B95670, vehicle_get_human_tank_definition, vehicle_definition_index);

	const s_vehicle_definition* vehicle_definition = TAG_GET(VEHICLE_TAG, const s_vehicle_definition, vehicle_definition_index);
	const s_vehicle_human_tank_definition* human_tank_definition = (const s_vehicle_human_tank_definition*)vehicle_definition->vehicle.physics_types.tag_blocks[_vehicle_type_human_tank].address;
	return human_tank_definition;
}

const s_vehicle_mantis_definition* __cdecl vehicle_get_mantis_definition(int32 vehicle_definition_index)
{
	//return INVOKE(0x00B95690, vehicle_get_mantis_definition, vehicle_definition_index);

	const s_vehicle_definition* vehicle_definition = TAG_GET(VEHICLE_TAG, const s_vehicle_definition, vehicle_definition_index);
	const s_vehicle_mantis_definition* mantis_definition = (const s_vehicle_mantis_definition*)vehicle_definition->vehicle.physics_types.tag_blocks[_vehicle_type_mantis].address;
	return mantis_definition;
}

const s_vehicle_turret_definition* __cdecl vehicle_get_turret_definition(int32 vehicle_definition_index)
{
	//return INVOKE(0x00B956B0, vehicle_get_turret_definition, vehicle_definition_index);

	const s_vehicle_definition* vehicle_definition = TAG_GET(VEHICLE_TAG, const s_vehicle_definition, vehicle_definition_index);
	const s_vehicle_turret_definition* turret_definition = (const s_vehicle_turret_definition*)vehicle_definition->vehicle.physics_types.tag_blocks[_vehicle_type_turret].address;
	return turret_definition;
}

const s_vehicle_vtol_definition* __cdecl vehicle_get_vtol_definition(int32 vehicle_definition_index)
{
	//return INVOKE(0x00B956D0, vehicle_get_vtol_definition, vehicle_definition_index);

	const s_vehicle_definition* vehicle_definition = TAG_GET(VEHICLE_TAG, const s_vehicle_definition, vehicle_definition_index);
	const s_vehicle_vtol_definition* vtol_definition = (const s_vehicle_vtol_definition*)vehicle_definition->vehicle.physics_types.tag_blocks[_vehicle_type_vtol].address;
	return vtol_definition;
}

void _vehicle_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(suspension_sound);
	UPDATE_REFERENCE_NAME(special_effect);
	UPDATE_REFERENCE_NAME(driver_boost_damage_effect_or_response);
	UPDATE_REFERENCE_NAME(rider_boost_damage_effect_or_response);

	UPDATE_STRUCT_REFERENCE_NAMES(physics_types);
}

void s_vehicle_physics_types::update_reference_names()
{
	if (tag_blocks[_vehicle_type_human_tank].count)
	{
		s_vehicle_human_tank_definition* human_tank_definition = (s_vehicle_human_tank_definition*)tag_blocks[_vehicle_type_human_tank].address;
		human_tank_definition->update_reference_names();
	}
}

void s_vehicle_engine_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(gear_shift_sound);
}

void s_vehicle_human_tank_definition::update_reference_names()
{
	UPDATE_STRUCT_REFERENCE_NAMES(engine);
}

void vehicle_definition::update_reference_names()
{
	UPDATE_STRUCT_REFERENCE_NAMES(vehicle);
}

