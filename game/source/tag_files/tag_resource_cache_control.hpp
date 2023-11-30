#pragma once

#include "cseries/cseries.hpp"

struct s_data_array;
struct c_tag_resource_cache_controller
{
	byte __data0[0x8];
	s_data_array* m_resource_control_data;
	byte __dataC[0x10];
	c_wrapped_flags __wrapped_flags1C;
	byte __data[0x3C];
};
static_assert(sizeof(c_tag_resource_cache_controller) == 0x60);

