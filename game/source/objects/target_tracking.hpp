#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_target.hpp"

struct c_tracked_target
{
	c_aim_target_object m_aim_target;

	short __unknownC;
	short __unknownE;

	real looking_theta;

	short __unknown14;
	short __unknown16;

	bool m_locked;
};
static_assert(sizeof(c_tracked_target) == 0x1C);

struct c_target_tracking_system
{
	c_static_array<c_tracked_target, 2> m_tracked_targets;
	long m_best_target_index;
	long __unknown8;
};
static_assert(sizeof(c_target_tracking_system) == 0x40);

