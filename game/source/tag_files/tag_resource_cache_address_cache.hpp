#pragma once

#include "cseries/cseries.hpp"

struct c_tag_resource_address_cache
{
	byte __data[0x44];
};
static_assert(sizeof(c_tag_resource_address_cache) == 0x44);

