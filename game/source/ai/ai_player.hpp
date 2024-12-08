#pragma once

#include "cseries/cseries.hpp"

struct ai_player_state
{
	long player_index;
	long recent_vehicle_index;
	short recent_seat_index;
	short recent_vehicle_ticks;
	word ticks_since_shooting;
	short ticks_since_player_threatening;
	long last_friendly_vehicle_shoot_time;
	long last_friendly_vehicle_shoot_index;
	char last_vehicle_airborne_ticks;
	bool crazy_vehicle_vocalized;
	bool needs_vehicle;
	long needs_vehicle_time;
	real_vector3d battle_vector;
};
static_assert(sizeof(ai_player_state) * 0x2C);
static_assert(sizeof(ai_player_state) * 4 == 0xB0);

extern bool __cdecl ai_player_state_needs_vehicle(short ai_player_index);

extern void debug_render_player_battle_vector();
extern void debug_render_player_needs_vehicle();

