#pragma once

#include "cseries/cseries.hpp"

struct s_vision_mode_state
{
	byte __data[0xF0];
};
static_assert(sizeof(s_vision_mode_state) == 0xF0);

