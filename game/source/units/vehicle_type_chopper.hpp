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

	c_animation_id __unknown14;
	c_animation_id __unknown18;
	byte __data1C[0x1C];

	c_vehicle_stop_counter m_stop_counter;
	c_vehicle_braking_counter m_braking_counter;

	byte __data[0x60 - 0x3A];
};
static_assert(sizeof(c_vehicle_type_chopper) == 0x60);

