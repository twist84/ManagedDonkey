#pragma once

#include "cseries/cseries.hpp"

struct ai_player_state
{
	long player_index;

	// unit/vehicle
	long eviction_unit_index;
	short eviction_seat_index;
	short eviction_ticks;

	short ticks_since_shooting;
	short ticks_since_threatening;

	long root_object_update_time;
	long root_object_index;

	char __unknown18;
	bool __unknown19;
	bool __unknown1A;
	byte __data1B[0x1];

	long vehicle_update_time;
	vector3d battle_vector;
};
static_assert(sizeof(ai_player_state) * 4 == 0xB0);

