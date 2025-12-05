#pragma once

#include "cseries/cseries.hpp"
#include "cseries/location.hpp"

struct s_object_activation_region :
	s_datum_header
{
	// all names subject to change

	// alignment?
	byte __data2[0x2];

	int32 activation_time;
	real_rectangle3d region_bounds;
	int32 havok_group; // e_havok_group
	s_location region_location;

	// pad?
	byte __data26[0x2];
};
static_assert(sizeof(s_object_activation_region) == 0x28);

extern void __cdecl object_activation_regions_dispose();
extern void __cdecl object_activation_regions_dispose_from_old_map();
extern void __cdecl object_activation_regions_initialize();
extern void __cdecl object_activation_regions_initialize_for_new_map();
extern void __cdecl object_activation_regions_update();

