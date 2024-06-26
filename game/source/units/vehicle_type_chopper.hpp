#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_engine.hpp"
#include "units/vehicle_utilities.hpp"

struct c_vehicle_type_chopper
{
public:
	s_vehicle_engine* const get_engine(long vehicle_index)
	{
		return &m_engine;
	}

protected:
	s_vehicle_engine m_engine;
	c_animation_id m_ground_speed_animation_id;
	c_animation_id m_steering_animation_id;

	byte __data1C[0xC];

	// STRING_ID(global, front_left_tire_position)
	// STRING_ID(global, front_right_tire_position)
	// STRING_ID(global, back_left_tire_position)
	// STRING_ID(global, back_right_tire_position)
	real __unknown28; // vehicle_function_safe_divide(__unknown28, engine_definition->wheel_circumferance)

	real __unknown2C;
	real __unknown30;
	byte __data34[0x4];

	c_vehicle_stop_counter m_stop_counter;
	c_vehicle_braking_counter m_braking_counter;

	byte __unknown3A; // bool?

	byte __data[0x60 - 0x3B];
};
static_assert(sizeof(c_vehicle_type_chopper) == 0x60);

