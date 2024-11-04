#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct light_datum :
	s_datum_header
{
	byte __data[0xE2];
};
static_assert(sizeof(light_datum) == 0xE4);

struct lights_game_globals_definition
{
	bool can_use_flashlights;
	long can_use_flashlights_time;
	bool __unknown8;
	byte __data9[0x3];

	// bitvector/static flags where each active bit corresponds to a light datum
	byte __dataC[0x34];
};
static_assert(sizeof(lights_game_globals_definition) == 0x40);

struct s_nondeterministic_light_data
{
	byte __data[0x2580];
};
static_assert(sizeof(s_nondeterministic_light_data) == 0x2580);

extern void __cdecl lights_prepare_for_window(long player_window_index);
extern void __cdecl lights_update();

