#pragma once

#include "cseries/cseries.hpp"

struct c_atmosphere_fog_interface
{
	byte __data[0x14];
};
static_assert(sizeof(c_atmosphere_fog_interface) == 0x14);

