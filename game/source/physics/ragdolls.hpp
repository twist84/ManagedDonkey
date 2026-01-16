#pragma once

#include "cseries/cseries.hpp"

struct s_ragdoll_datum :
	s_datum_header
{
	byte __data[0x12E];
};
COMPILE_ASSERT(sizeof(s_ragdoll_datum) == 0x130);

