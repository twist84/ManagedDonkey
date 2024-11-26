#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_auto_turret.hpp"

struct c_vehicle_type_guardian
{
public:

protected:
	real m_steering;
	real m_hover;
	c_vehicle_auto_turret m_auto_turret;
};
static_assert(sizeof(c_vehicle_type_guardian) == 0x38);

