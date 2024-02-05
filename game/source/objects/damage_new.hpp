#pragma once

#include "cseries/cseries.hpp"

struct s_damage_globals
{
	struct s_damage_acceleration
	{
		real_point3d point;
		vector3d acceleration;
		long object_index;
		short node_index;
		word_flags flags;
	};
	static_assert(sizeof(s_damage_acceleration) == 0x20);

	long damage_acceleration_count;
	c_static_array<s_damage_acceleration, 64> damage_accelerations;
	c_static_flags<64> damage_accelerations_evictable;
	bool damage_acceleration_queue_active;
};
static_assert(sizeof(s_damage_globals) == 0x810);

