#pragma once

#include "cseries/cseries.hpp"

struct c_aim_target_object
{
	bool __unknown0;
	dword target_player;
	dword target_object;
};
static_assert(sizeof(c_aim_target_object) == 0xC);

struct s_aim_assist_targeting_result
{
	c_aim_target_object aim_arget;
	dword model_target;
	real primary_autoaim_level;
	real secondary_autoaim_level;
	vector3d lead_vector;
	dword_flags flags;
};
static_assert(sizeof(s_aim_assist_targeting_result) == 0x28);

