#include "units/vehicle_definitions.hpp"

void _vehicle_definition::update_reference_names()
{
	suspension_sound.get_name();
	special_effect.get_name();
	driver_boost_damage_effect_or_response.get_name();
	rider_boost_damage_effect_or_response.get_name();

	physics_types.update_reference_names();
}

void s_vehicle_physics_types::update_reference_names()
{
	UPDATE_REFERENCE_NAMES(type_human_tank);
}

void s_vehicle_engine_definition::update_reference_names()
{
	gear_shift_sound.get_name();
}

void s_vehicle_human_tank_definition::update_reference_names()
{
	engine.update_reference_names();
}
