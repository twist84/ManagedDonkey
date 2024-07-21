#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_target.hpp"

struct s_aim_assist_targeting_result
{
	c_aim_target_object aim_target;
	long target_player;
	real primary_autoaim_level;
	real secondary_autoaim_level;
	vector3d lead_vector;
	dword_flags flags;
};
static_assert(sizeof(s_aim_assist_targeting_result) == 0x28);

extern void aim_assist_debug_render();

