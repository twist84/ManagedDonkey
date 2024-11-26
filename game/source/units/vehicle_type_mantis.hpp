#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_type_mantis
{
public:

protected:
	real m_current_leg_turn;
	vector3d m_last_up_vector;
	vector2d m_foot_motion;
	real m_turn_control;
	real m_steering;
	real m_speed;
	real m_slide;
	char m_planted_leg_group;
	char m_legs_planted_ticks;
	long m_jump_time;
};
static_assert(sizeof(c_vehicle_type_mantis) == 0x30);

