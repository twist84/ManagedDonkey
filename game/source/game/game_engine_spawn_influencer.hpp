#pragma once

#include "cseries/cseries.hpp"

struct s_dead_player_info
{
	long player_index;
	dword time;
	real_point3d position;
};
static_assert(sizeof(s_dead_player_info) == 0x14);

