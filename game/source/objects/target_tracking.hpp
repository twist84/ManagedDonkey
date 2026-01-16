#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_target.hpp"

class c_tracked_target
{
public:
	c_aim_target_object m_aim_target;

	int16 __unknownC;
	int16 __unknownE;

	real32 looking_theta;

	int16 __unknown14;
	int16 __unknown16;

	bool m_locked;
};
COMPILE_ASSERT(sizeof(c_tracked_target) == 0x1C);

class c_target_tracking_system
{
public:
	c_static_array<c_tracked_target, 2> m_tracked_targets;
	int32 m_best_target_index;
	int32 __unknown8;
};
COMPILE_ASSERT(sizeof(c_target_tracking_system) == 0x40);

