#pragma once

#include "cseries/cseries.hpp"

struct s_network_quality_statistics
{
	byte __data[0xC0];
};
static_assert(sizeof(s_network_quality_statistics) == 0xC0);

