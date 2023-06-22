#pragma once

#include "cseries/cseries.hpp"

struct c_simulation_watcher
{
	byte __data[0x16658];
};
static_assert(sizeof(c_simulation_watcher) == 0x16658);

