#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_type_human_plane
{
public:

protected:
	real m_speed;
	real m_slide;
	real m_rise;
	real m_steering;
	real m_turn_animation_position;
	c_animation_id m_turn_animation_id;
};
static_assert(sizeof(c_vehicle_type_human_plane) == 0x18);

