#pragma once

#include "cseries/cseries.hpp"

struct c_simulation_queue
{
	byte __data[0x1C];
};
static_assert(sizeof(c_simulation_queue) == 0x1C);