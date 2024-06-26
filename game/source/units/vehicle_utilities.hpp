#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_stop_counter
{
	byte m_value;
};
static_assert(sizeof(c_vehicle_stop_counter) == sizeof(byte));

struct c_vehicle_braking_counter
{
	byte m_value;
};
static_assert(sizeof(c_vehicle_braking_counter) == sizeof(byte));


