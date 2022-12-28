#pragma once

#include "cseries/cseries.hpp"

struct s_determinism_verification
{
	dword unchanged_field_mask;
	long game_state_check;
	long cheats_check;
	c_static_array<long, 30> consumer_check;
};
static_assert(sizeof(s_determinism_verification) == 0x84);

