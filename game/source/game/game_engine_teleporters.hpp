#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_area_set.hpp"

struct c_teleporter_area : c_area
{
	long m_recharge_timer;
	long m_blocked_message_delay;
	long m_blocked_ticks;
};
static_assert(sizeof(c_teleporter_area) == 0x70);

// g_teleporter_globals
struct s_teleporter_globals
{
	c_area_set<c_teleporter_area, 32> teleporters;
};
static_assert(sizeof(s_teleporter_globals) == 0xE10);

