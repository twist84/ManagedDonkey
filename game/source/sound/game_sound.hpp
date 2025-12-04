#pragma once

#include "cseries/cseries.hpp"

struct game_looping_sound_datum :
	s_datum_header
{
	byte __data[0x1E];
};
static_assert(sizeof(game_looping_sound_datum) == 0x20);

struct s_game_sound_listener
{
	int32 cluster_loop_index;
	int32 active_fog_definition_index;
	bool tracked_as_underwater;
};
static_assert(sizeof(s_game_sound_listener) == 0xC);

struct s_sound_global_environment_parameters
{
	real32 direct_gain;
	real32 direct_hf_gain;
	real32 reverb_gain;
	real32 reverb_hf_gain;
	int32 speaker_filter_frequency;
	real32 speaker_filter_gain;
};
static_assert(sizeof(s_sound_global_environment_parameters) == 0x18);

struct s_game_sound_globals
{
	c_static_array<s_game_sound_listener, 4> listeners;
	int32 dialog_ducker_stop_tick;
	c_static_array<uns16, 128> portal_occlusions;
	s_sound_global_environment_parameters global_environment_parameters;
	bool allow_details_in_cinematics;
	bool suppress_ambience_update_on_revert;
	real32 additional_occlusion_interpolator_dt;
};
static_assert(sizeof(s_game_sound_globals) == 0x154);

struct s_game_sound_impulse_datum
{
	byte __data[0x200];
};
static_assert(sizeof(s_game_sound_impulse_datum) == 0x200);

extern void __cdecl game_sound_dispose();
extern void __cdecl game_sound_dispose_from_old_map();
extern void __cdecl game_sound_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl game_sound_initialize();
extern void __cdecl game_sound_initialize_for_new_map();
extern void __cdecl game_sound_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern void __cdecl game_sound_process_update_messages();
extern void __cdecl game_sound_update(real32 game_seconds_elapsed);
extern int32 __cdecl object_impulse_sound_new(int32 object_index, int32 definition_index, int16 node_index, const real_point3d* position, const real_vector3d* forward, real32 scale);
extern void __cdecl scripted_looping_sound_start_with_effect(int32 definition_index, int32 source_object_index, real32 scale, int32 playback_label);
extern void __cdecl scripting_looping_sound_spam();
extern int32 __cdecl unspatialized_impulse_sound_new(int32 definition_index, real32 scale);

