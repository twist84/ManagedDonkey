#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_engine.hpp"
#include "units/vehicle_utilities.hpp"

struct c_vehicle_type_human_tank
{
public:
	s_vehicle_engine* const get_engine(long vehicle_index);

protected:
	s_vehicle_engine m_engine;

	c_animation_id __animation_id14; // m_steering_animation_id?
	real __unknown18;

	// STRING_ID(global, left_tread_position)
	real __unknown1C; // vehicle_function_safe_divide(this->__unknown1C, human_tank_definition->wheel_circumferance);

	// STRING_ID(global, right_tread_position)
	real __unknown20; // vehicle_function_safe_divide(this->__unknown20, human_tank_definition->wheel_circumferance);

	c_vehicle_stop_counter m_stop_counter;

	byte __data[0x60 - 0x25];
};
static_assert(sizeof(c_vehicle_type_human_tank) == 0x60);

