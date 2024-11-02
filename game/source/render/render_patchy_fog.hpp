#pragma once

#include "cseries/cseries.hpp"

struct c_patchy_fog
{
	static void __cdecl frame_advance_all(real seconds_elapsed);

	real_point3d __point0;
	vector3d __vectorC;
	vector3d __vector18;
	real __unknown24;
	real __unknown28;
	real __unknown2C;
	long __unknown30;
	c_static_array<real, 100> __unknown34;
	c_static_array<real, 100> __unknown1C4;
};
static_assert(sizeof(c_patchy_fog) == 0x354);

