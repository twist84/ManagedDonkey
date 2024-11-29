#pragma once

#include "cseries/cseries.hpp"

struct game_allegiance
{
	short team1_index;
	short team2_index;
	short incident_threshold;
	short incident_decay_time;
	bool team1_suspicious;
	bool team2_suspicious;
	bool currently_broken;
	bool status_changed;
	short current_incidents;
	short current_incident_decay_time;
	long last_incident_time;
};
static_assert(sizeof(game_allegiance) == 0x14);

struct s_game_allegiance_globals
{
	short allegiance_count;
	game_allegiance allegiances[16];
	c_static_flags<256> ally_bitvector;
	c_static_flags<256> friendly_bitvector;
};
static_assert(sizeof(s_game_allegiance_globals) == 0x184);

extern bool __cdecl game_allegiance_betrayal_exists();
extern void __cdecl game_allegiance_dispose();
extern void __cdecl game_allegiance_dispose_from_old_map();
extern void __cdecl game_allegiance_initialize();
extern void __cdecl game_allegiance_initialize_for_new_map();
extern void __cdecl game_allegiance_update();

