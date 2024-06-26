#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_type_turret
{
public:

protected:
	// #TODO: map this

	byte __data[0x60];
};
static_assert(sizeof(c_vehicle_type_turret) == 0x60);

