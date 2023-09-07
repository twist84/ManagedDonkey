#pragma once

#include "cseries/cseries.hpp"

struct s_object_broadphase
{
	byte __data[0x32450];
};
static_assert(sizeof(s_object_broadphase) == 0x32450);

