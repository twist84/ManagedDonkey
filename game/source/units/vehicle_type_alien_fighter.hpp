#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_type_alien_fighter
{
public:

protected:
	c_animation_id m_steering_animation_id;

	// STRING_ID(global, thrust)
	// STRING_ID(global, wingtip_contrail_new)
	real __unknown4;

	// STRING_ID(global, engine_rpm)
	real __unknown8;

	real __unknownC;

	// STRING_ID(global, engine_rpm)
	real __unknown10;

	real __unknown14;

	byte m_trick_type; // e_vehicle_trick_type
	byte m_trick_duration_ticks;

	byte __data[0x60 - 0x1A];
};
static_assert(sizeof(c_vehicle_type_alien_fighter) == 0x60);

