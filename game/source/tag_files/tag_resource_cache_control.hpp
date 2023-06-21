#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct c_tag_resource_cache_controller
{
	byte __data0[0x8];
	s_data_array* __data_array;
	byte __dataC[0x54];
};
static_assert(sizeof(c_tag_resource_cache_controller) == 0x60);

