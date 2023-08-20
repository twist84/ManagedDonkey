#pragma once

#include "units/vehicle_engine.hpp"

struct c_vehicle_type_component :
	public s_vehicle_engine
{
	s_vehicle_engine* const get_engine(long vehicle_index);
};

