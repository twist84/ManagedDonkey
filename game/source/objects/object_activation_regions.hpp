#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_object_activation_region : s_datum_header
{
	byte __data[0x26];
};
static_assert(sizeof(s_object_activation_region) == 0x28);

