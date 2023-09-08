#pragma once

#include "cseries/cseries.hpp"

struct ai_player_state
{
	long player_index;

	// unit/vehicle
	long eviction_unit_index;
	short eviction_seat_index;
	short eviction_ticks;

	short __unknownC;
	word __unknownE;

	dword root_object_update_time;
	long root_object_index;

	byte __data18[0x8];

	vector3d battle_vector;
};
static_assert(sizeof(ai_player_state) * 4 == 0xB0);

