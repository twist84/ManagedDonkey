#pragma once

#include "tag_files/tag_groups.hpp"

enum
{
	// when used as a background stereo track, causes nearby AIs to be unable to hear
	_looping_sound_deafening_bit = 0,

	// this is a collection of permutations strung together that should play once then stop.
	_looping_sound_behave_like_impulse_sound_bit,

	// all other music loops will stop when this one starts.
	_looping_sound_stops_music_bit,

	// always play as 3d sound, even in first person
	_looping_sound_always_spatialized_bit,

	// synchronizes playback with other looping sounds attached to the owner of this sound
	_looping_sound_synchronize_bit,

	_looping_sound_synchronize_tracks_bit,
	_looping_sound_fake_spatialized_bit,
	_looping_sound_combine_all_3d_instances_bit,

	_looping_sound_bit8,

	NUMBER_OF_LOOPING_SOUND_FLAGS,
};

struct looping_sound_track
{
	string_id name;
	uns32 flags;
	real_decibel gain_db;
	real32 fade_in_duration; // seconds

	// ODST
	int16 fade_in_mode;
	int16 fade_in_pad;

	real32 fade_out_duration; // seconds

	// ODST
	int16 fade_out_mode;
	int16 fade_out_pad;

	s_tag_reference start_sound;
	s_tag_reference loop_sound;
	s_tag_reference stop_sound;
	s_tag_reference alternate_loop_sound;
	s_tag_reference alternate_stop_sound;

	uns16 output_effect;
	uns16 pad;

	s_tag_reference alternate_transition_in_sound;
	s_tag_reference alternate_transition_out_sound;

	real32 alternate_crossfade_duration; // seconds

	// ODST
	int16 alternate_fade_in_mode;
	int16 alternate_fade_in_pad;

	real32 alternate_fade_out_duration; // seconds

	// ODST
	int16 alternate_fade_out_mode;
	int16 alternate_fade_out_pad;
};
COMPILE_ASSERT(sizeof(looping_sound_track) == 0xA0);

struct looping_sound_detail
{
	string_id name;
	s_tag_reference sound;

	// frequency of play

	// the time between successive playings of this sound will be randomly selected from this range.
	real32 period_lower_bound; // seconds
	real32 period_upper_bound; // seconds

	real_decibel gain_db;
	uns32 flags;

	// random spatialization
	real32 theta_lower_bound; // yaw
	real32 theta_upper_bound; // yaw
	real32 phi_lower_bound; // pitch
	real32 phi_upper_bound; // pitch
	real32 distance_lower_bound;
	real32 distance_upper_bound;
};
COMPILE_ASSERT(sizeof(looping_sound_detail) == 0x3C);

typedef struct looping_sound_definition
{
	uns32 flags;

	// marty's music time
	real32 music_play_time; // seconds
	real32 unused_marty_love;

	real32 runtime_minimum_distance;
	real32 runtime_maximum_distance;

	s_tag_reference continuous_damage_effect; // unused

	int16 runtime_sound_class;
	int16 unused_pad;

	// tracks play in parallel and loop continuously for the duration of the looping sound.
	c_typed_tag_block<looping_sound_track> tracks;

	// detail sounds play at random throughout the duration of the looping sound.
	c_typed_tag_block<looping_sound_detail> details;
} s_looping_sound_definition;
COMPILE_ASSERT(sizeof(looping_sound_definition) == 0x40);

struct s_sound_globals_definition
{
	c_typed_tag_reference<SOUND_CLASSES_TAG, INVALID_TAG> sound_classes;
	c_typed_tag_reference<SOUND_EFFECT_COLLECTION_TAG, INVALID_TAG> sound_effects;
	c_typed_tag_reference<SOUND_MIX_TAG, INVALID_TAG> sound_mix;
	c_typed_tag_reference<SOUND_DIALOGUE_CONSTANTS_TAG, INVALID_TAG> sound_combat_dialogue_constants;
	c_typed_tag_reference<SOUND_GLOBAL_PROPAGATION_TAG, INVALID_TAG> sound_propagation;
	c_typed_tag_reference<SOUND_UI_SOUNDS_TAG, INVALID_TAG> gfx_ui_sounds;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_sound_globals_definition) == 0x60);

