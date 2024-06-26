#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_type_vtol
{
public:

protected:
	// #TODO: map this

	byte __data[0x60];
};
static_assert(sizeof(c_vehicle_type_vtol) == 0x60);

