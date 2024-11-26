#pragma once

#include "cseries/cseries.hpp"
#include "units/vehicle_auto_turret.hpp"

struct c_vehicle_type_turret
{
public:

protected:
	c_vehicle_auto_turret m_auto_turret;
};
static_assert(sizeof(c_vehicle_type_turret) == sizeof(c_vehicle_auto_turret));

