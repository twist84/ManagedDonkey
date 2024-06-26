#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_type_human_plane
{
public:

protected:
	real __unknown0;
	real __unknown4;
	real __unknown8;
	real __unknownC;
	real __unknown10;
	c_animation_id m_steering_animation_id;

	byte __data[0x60 - 0x18];
};
static_assert(sizeof(c_vehicle_type_human_plane) == 0x60);

