#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_engine.hpp"
#include "units/vehicle_utilities.hpp"

struct c_vehicle_type_human_tank
{
public:
	s_vehicle_engine* const get_engine(long vehicle_index)
	{
		return &m_engine;
	}

protected:
	s_vehicle_engine m_engine;

	c_animation_id __animation_id14;
	byte __data18[0xC];

	c_vehicle_stop_counter m_stop_counter;

	byte __data[0x60 - 0x25];
};
static_assert(sizeof(c_vehicle_type_human_tank) == 0x60);

