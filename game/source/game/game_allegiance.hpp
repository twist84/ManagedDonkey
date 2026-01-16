#pragma once

#include "cseries/cseries.hpp"

struct game_allegiance
{
	int16 team1_index;
	int16 team2_index;
	int16 incident_threshold;
	int16 incident_decay_time;
	bool team1_suspicious;
	bool team2_suspicious;
	bool currently_broken;
	bool status_changed;
	int16 current_incidents;
	int16 current_incident_decay_time;
	int32 last_incident_time;
};
COMPILE_ASSERT(sizeof(game_allegiance) == 0x14);

struct s_game_allegiance_globals
{
	int16 allegiance_count;
	game_allegiance allegiances[16];
	c_static_flags<256> ally_bitvector;
	c_static_flags<256> friendly_bitvector;
};
COMPILE_ASSERT(sizeof(s_game_allegiance_globals) == 0x184);

extern bool __cdecl game_allegiance_betrayal_exists();
extern void __cdecl game_allegiance_dispose();
extern void __cdecl game_allegiance_dispose_from_old_map();
extern void __cdecl game_allegiance_initialize();
extern void __cdecl game_allegiance_initialize_for_new_map();
extern void __cdecl game_allegiance_update();

