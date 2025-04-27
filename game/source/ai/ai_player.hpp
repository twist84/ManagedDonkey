#pragma once

#include "cseries/cseries.hpp"

struct ai_player_state
{
	int32 player_index;
	int32 recent_vehicle_index;
	int16 recent_seat_index;
	int16 recent_vehicle_ticks;
	uint16 ticks_since_shooting;
	int16 ticks_since_player_threatening;
	int32 last_friendly_vehicle_shoot_time;
	int32 last_friendly_vehicle_shoot_index;
	char last_vehicle_airborne_ticks;
	bool crazy_vehicle_vocalized;
	bool needs_vehicle;
	int32 needs_vehicle_time;
	real_vector3d battle_vector;
};
static_assert(sizeof(ai_player_state) * 0x2C);
static_assert(sizeof(ai_player_state) * 4 == 0xB0);

extern void __cdecl ai_player_initialize();
extern bool __cdecl ai_player_state_needs_vehicle(int16 ai_player_index);
extern void __cdecl ai_player_state_update();
extern void __cdecl ai_players_initialize_for_new_map();
extern void __cdecl ai_players_reset();

extern void debug_render_player_battle_vector();
extern void debug_render_player_needs_vehicle();

