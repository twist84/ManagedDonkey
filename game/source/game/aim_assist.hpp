#pragma once

#include "cseries/cseries.hpp"

struct s_aim_assist_targeting_result
{
	bool __unknown0;
	dword target_player_index;
	dword target_object_index;
	dword model_target;
	real primary_autoaim_level;
	real secondary_autoaim_level;
	vector3d lead_vector;
	dword_flags flags;
};
static_assert(sizeof(s_aim_assist_targeting_result) == 0x28);

