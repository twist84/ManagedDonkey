#pragma once

#include "cseries/cseries.hpp"

struct s_animation_sync_action
{
	c_string_id name;
	real_point3d origin;
	vector3d forward;
	vector3d up;
};
static_assert(sizeof(s_animation_sync_action) == 0x28);

