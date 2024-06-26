#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_auto_turret.hpp"

struct c_vehicle_type_turret
{
public:

protected:
	// #TODO: map this

	c_vehicle_auto_turret m_auto_turret;

	byte __data[0x60 - 0x30];
};
static_assert(sizeof(c_vehicle_type_turret) == 0x60);

