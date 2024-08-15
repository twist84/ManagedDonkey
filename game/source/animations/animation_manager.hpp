#pragma once

#include "animations/animation_interpolation.hpp"
#include "animations/mixing_board/channels/animation_channel.hpp"
#include "cseries/cseries.hpp"

struct c_animation_manager
{
	c_animation_channel __unknown0;
	c_animation_channel __unknown3C;
	c_interpolator_control __unknown78;
	byte __data84[0x3C];
};
static_assert(sizeof(c_animation_manager) == 0xC0);

