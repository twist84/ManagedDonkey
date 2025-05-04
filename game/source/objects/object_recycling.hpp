#pragma once

#include "cseries/cseries.hpp"

struct s_recycling_volumes
{
	byte __data[0x148];
};
static_assert(sizeof(s_recycling_volumes) == 0x148);

struct s_recycling_group :
	s_datum_header
{
	byte __data[0x12];
};
static_assert(sizeof(s_recycling_group) == 0x14);

