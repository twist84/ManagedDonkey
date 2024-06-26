#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_auto_turret.hpp"

struct c_vehicle_type_guardian
{
public:

protected:
	real __unknown0;
	real __unknown4;

	c_vehicle_auto_turret m_auto_turret;

	byte __data[0x60 - 0x38];
};
static_assert(sizeof(c_vehicle_type_guardian) == 0x60);

