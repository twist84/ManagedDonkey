#pragma once

#include "cseries/cseries.hpp"

struct c_vehicle_type_mantis
{
public:

protected:

	byte __data0[0x4];
	vector3d __unknown4;
	byte __data10[0x8];
	real __unknown18;
	real __unknown1C;
	real __unknown20;
	real __unknown24;
	char __unknown28;
	char __unknown29;
	byte __data2A[0x2];
	long __unknown2C;

	byte __data[0x60 - 0x30];
};
static_assert(sizeof(c_vehicle_type_mantis) == 0x60);

