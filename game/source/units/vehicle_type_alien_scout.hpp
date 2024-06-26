#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_type_alien_scout
{
public:

protected:
	real __unknown0;

	// STRING_ID(global, engine_power)
	real __unknown4;

	// STRING_ID(global, wingtip_contrail)
	real __unknown8;

	real __unknownC;

	// STRING_ID(global, hover)
	real __unknown10;

	// STRING_ID(global, gear_position)
	real __unknown14;

	real __unknown18;

	c_animation_id m_steering_animation_id;

	byte __data[0x60 - 0x20];
};
static_assert(sizeof(c_vehicle_type_alien_scout) == 0x60);

