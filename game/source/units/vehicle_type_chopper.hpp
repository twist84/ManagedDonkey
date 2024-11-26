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
	c_animation_id m_ground_speed_animation_id;
	c_animation_id m_turn_animation_id;
	vector3d m_last_relative_angular_velocity;
	real m_wheel;
	real m_wheel_speed;
	real m_turn;
	real m_bank;
	c_vehicle_stop_counter m_stop_counter;
	c_vehicle_braking_counter m_braking_counter;
	bool m_control_e_brake;
};
static_assert(sizeof(c_vehicle_type_chopper) == 0x3C);

