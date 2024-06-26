#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_object_activation_region :
	s_datum_header
{
	// all names subject to change

	// alignment?
	byte __data2[0x2];

	long activation_time;
	real_rectangle3d region_bounds;
	long havok_group; // e_havok_group
	s_location region_location;

	// pad?
	byte __data26[0x2];
};
static_assert(sizeof(s_object_activation_region) == 0x28);

extern void __cdecl object_activation_regions_update();

