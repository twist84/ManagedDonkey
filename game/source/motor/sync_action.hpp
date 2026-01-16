#pragma once

#include "cseries/cseries.hpp"

struct s_animation_sync_action
{
	c_string_id name;
	real_point3d origin;
	real_vector3d forward;
	real_vector3d up;
};
COMPILE_ASSERT(sizeof(s_animation_sync_action) == 0x28);

extern bool& debug_enable_force_phonebooth_assassinate;

