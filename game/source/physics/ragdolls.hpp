#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_ragdoll_datum :
	s_datum_header
{
	byte __data[0x12E];
};
static_assert(sizeof(s_ragdoll_datum) == 0x130);

