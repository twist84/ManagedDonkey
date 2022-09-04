#pragma once

#include "cseries/cseries.hpp"

struct c_simulation_actor
{
	byte __data[0x94];
};
static_assert(sizeof(c_simulation_actor) == 0x94);