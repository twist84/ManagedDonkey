#pragma once

#include "cseries/cseries.hpp"
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
	c_animation_id m_ground_speed_animation_id;
	c_animation_id m_ground_rear_animation_id;
	c_animation_id m_turn_animation_id;
	vector3d m_grounded_up_vector;
	real m_wheel;
	real m_rear_wheel;
	real m_turn;
	c_vehicle_stop_counter m_stop_counter;
	c_vehicle_braking_counter m_braking_counter;
	byte m_control_e_brake;
};
static_assert(sizeof(c_vehicle_type_human_jeep) == 0x3C);

