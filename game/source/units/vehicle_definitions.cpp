#include "units/vehicle_definitions.hpp"

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
	UPDATE_BLOCK_REFERENCE_NAMES(type_human_tank);
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

