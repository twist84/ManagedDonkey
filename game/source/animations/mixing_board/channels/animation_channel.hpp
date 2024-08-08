#pragma once

#include "cseries/cseries.hpp"

struct c_animation_channel
{
	byte __data[0x3C];
};
static_assert(sizeof(c_animation_channel) == 0x3C);

