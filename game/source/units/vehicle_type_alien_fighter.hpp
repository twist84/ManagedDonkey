#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_type_alien_fighter
{
public:

protected:
	c_animation_id m_turn_animation_id;
	real m_thrust;
	real m_speed;
	real m_turn;
	real m_slide;
	real m_speed_anti_gravity_fraction;
	char m_trick_type;
	byte m_trick_tick;
};
static_assert(sizeof(c_vehicle_type_alien_fighter) == 0x1C);

