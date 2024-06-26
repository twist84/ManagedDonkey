#pragma once

#include "cseries/cseries.hpp"
#include "objects/objects.hpp"
#include "units/vehicle_engine.hpp"
#include "units/vehicle_utilities.hpp"

struct c_vehicle_type_human_jeep
{
public:
	s_vehicle_engine* const get_engine(long vehicle_index)
	{
		return &m_engine;
	}

protected:
	s_vehicle_engine m_engine;

	c_animation_id __animation_id14;
	c_animation_id __animation_id18;
	c_animation_id __animation_id1C;

	vector3d m_up;

	real __unknown2C;
	real __unknown30;
	real __unknown34;

	c_vehicle_stop_counter m_stop_counter;
	c_vehicle_braking_counter m_braking_counter;

	byte __unknown3A;

	byte __data[0x60 - 0x3B];
};
static_assert(sizeof(c_vehicle_type_human_jeep) == 0x60);

