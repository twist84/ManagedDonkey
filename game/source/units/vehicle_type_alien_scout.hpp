#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_type_alien_scout
{
public:

protected:
	real m_engine_function_position;
	real m_engine_function_velocity;
	real m_boost_function_velocity;
	real m_steering;
	real m_hover;
	real m_gear_position;
	real m_turn_animation_position;
	c_animation_id m_turn_animation_id;
};
static_assert(sizeof(c_vehicle_type_alien_scout) == 0x20);

