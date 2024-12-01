#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct game_looping_sound_datum :
	s_datum_header
{
	byte __data[0x1E];
};
static_assert(sizeof(game_looping_sound_datum) == 0x20);

struct s_game_sound_listener
{
	long cluster_loop_index;
	long active_fog_definition_index;
	bool tracked_as_underwater;
};
static_assert(sizeof(s_game_sound_listener) == 0xC);

struct s_sound_global_environment_parameters
{
	real direct_gain;
	real direct_hf_gain;
	real reverb_gain;
	real reverb_hf_gain;
	long speaker_filter_frequency;
	real speaker_filter_gain;
};
static_assert(sizeof(s_sound_global_environment_parameters) == 0x18);

struct s_game_sound_globals
{
	c_static_array<s_game_sound_listener, 4> listeners;
	long dialog_ducker_stop_tick;
	c_static_array<word, 128> portal_occlusions;
	s_sound_global_environment_parameters global_environment_parameters;
	bool allow_details_in_cinematics;
	bool suppress_ambience_update_on_revert;
	real additional_occlusion_interpolator_dt;
};
static_assert(sizeof(s_game_sound_globals) == 0x154);

struct s_game_sound_impulse_datum
{
	byte __data[0x200];
};
static_assert(sizeof(s_game_sound_impulse_datum) == 0x200);

extern void __cdecl game_sound_dispose();
extern void __cdecl game_sound_dispose_from_old_map();
extern void __cdecl game_sound_dispose_from_old_structure_bsp(dword deactivating_structure_bsp_mask);
extern void __cdecl game_sound_initialize();
extern void __cdecl game_sound_initialize_for_new_map();
extern void __cdecl game_sound_initialize_for_new_structure_bsp(dword activating_structure_bsp_mask);
extern void __cdecl game_sound_process_update_messages();
extern void __cdecl game_sound_update(real game_seconds_elapsed);

