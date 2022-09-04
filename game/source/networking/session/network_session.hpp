#pragma once

#include "cseries/cseries.hpp"

struct c_network_session
{
	byte __data[0x25BC40];
};
static_assert(sizeof(c_network_session) == 0x25BC40);