#pragma once

#include "cseries/cseries.hpp"

struct rumble_data
{
	uns16 left_frequency;
	uns16 right_frequency;
};
COMPILE_ASSERT(sizeof(rumble_data) == 0x4);

struct player_rumble_impulse
{
	int32 damage_response_index;
	int32 class_index;
	real32 rumble_scale;
};
COMPILE_ASSERT(sizeof(player_rumble_impulse) == 0xC);

struct player_rumble_datum
{
	player_rumble_impulse impulse[8];
	real32 time_elapsed[8];
	real32 left_frequency_continuous;
	real32 right_frequency_continuous;
};
COMPILE_ASSERT(sizeof(player_rumble_datum) == 0x88);

struct scripted_rumble_datum
{
	real32 left_frequency;
	real32 right_frequency;
	real32 scale;
};
COMPILE_ASSERT(sizeof(scripted_rumble_datum) == 0xC);

struct rumble_global_data
{
	player_rumble_datum player_rumble_data[4];
	scripted_rumble_datum scripted_rumble;
};
COMPILE_ASSERT(sizeof(rumble_global_data) == 0x22C);

extern void __cdecl rumble_clear_all_now();
extern void __cdecl rumble_dispose();
extern void __cdecl rumble_dispose_from_old_map();
extern void __cdecl rumble_initialize();
extern void __cdecl rumble_initialize_for_new_map();
extern void __cdecl rumble_update(real32 world_seconds_elapsed);

