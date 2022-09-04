#pragma once

#include "cseries/cseries.hpp"

struct c_network_observer
{
	byte __data[0x23F20];
};
static_assert(sizeof(c_network_observer) == 0x23F20);