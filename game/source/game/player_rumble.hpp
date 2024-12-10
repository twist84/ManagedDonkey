#pragma once

#include "cseries/cseries.hpp"

struct rumble_data
{
	word left_frequency;
	word right_frequency;
};
static_assert(sizeof(rumble_data) == 0x4);

struct player_rumble_impulse
{
	long damage_response_index;
	long class_index;
	real rumble_scale;
};
static_assert(sizeof(player_rumble_impulse) == 0xC);

struct player_rumble_datum
{
	player_rumble_impulse impulse[8];
	real time_elapsed[8];
	real left_frequency_continuous;
	real right_frequency_continuous;
};
static_assert(sizeof(player_rumble_datum) == 0x88);

struct scripted_rumble_datum
{
	real left_frequency;
	real right_frequency;
	real scale;
};
static_assert(sizeof(scripted_rumble_datum) == 0xC);

struct rumble_global_data
{
	player_rumble_datum player_rumble_data[4];
	scripted_rumble_datum scripted_rumble;
};
static_assert(sizeof(rumble_global_data) == 0x22C);

extern void __cdecl rumble_clear_all_now();
extern void __cdecl rumble_dispose();
extern void __cdecl rumble_dispose_from_old_map();
extern void __cdecl rumble_initialize();
extern void __cdecl rumble_initialize_for_new_map();
extern void __cdecl rumble_update(real world_seconds_elapsed);

