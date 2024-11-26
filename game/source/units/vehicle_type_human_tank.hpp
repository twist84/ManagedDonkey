#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_engine.hpp"
#include "units/vehicle_utilities.hpp"

struct c_vehicle_type_human_tank
{
public:
	s_vehicle_engine* const get_engine(long vehicle_index);

protected:
	s_vehicle_engine m_engine;
	c_animation_id m_ground_speed_animation_id;
	real m_differential;
	real m_left_tread;
	real m_right_tread;
	c_vehicle_stop_counter m_stop_counter;
};
static_assert(sizeof(c_vehicle_type_human_tank) == 0x28);

