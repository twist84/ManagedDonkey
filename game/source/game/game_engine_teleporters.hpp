#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_area_set.hpp"

class c_teleporter_area :
	public c_area
{
public:
	int32 m_recharge_timer;
	int32 m_blocked_message_delay;
	int32 m_blocked_ticks;
};
COMPILE_ASSERT(sizeof(c_teleporter_area) == 0x70);

// g_teleporter_globals
struct s_teleporter_globals
{
	c_area_set<c_teleporter_area, 32> teleporters;
};
COMPILE_ASSERT(sizeof(s_teleporter_globals) == 0xE10);

