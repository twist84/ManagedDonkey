#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

enum e_game_engine_event_flags
{
	_game_engine_event_quantity_message_bit = 0,
	_game_engine_event_suppress_text_bit,

	k_game_engine_event_flags
};

enum e_game_engine_event_audience
{
	_game_engine_event_audience_cause_player = 0,
	_game_engine_event_audience_cause_team,
	_game_engine_event_audience_effect_player,
	_game_engine_event_audience_effect_team,
	_game_engine_event_audience_all,

	k_game_engine_event_audience_count
};

enum e_game_engine_event_response_context
{
	_game_engine_event_response_context_self = 0,
	_game_engine_event_response_context_friendly,
	_game_engine_event_response_context_enemy,
	_game_engine_event_response_context_neutral,

	k_game_engine_event_response_context_count
};

enum e_game_engine_event_input
{
	_game_engine_event_input_none = 0,
	_game_engine_event_input_cause_player,
	_game_engine_event_input_cause_team,
	_game_engine_event_input_effect_player,
	_game_engine_event_input_effect_team,

	k_game_engine_event_input_count
};

enum e_game_engine_event_splitscreen_suppression
{
	_game_engine_event_splitscreen_suppression_none = 0,
	_game_engine_event_splitscreen_suppression_suppress_audio,
	_game_engine_event_splitscreen_suppression_suppress_audio_if_overlapping,
	_game_engine_event_splitscreen_suppression_suppress_text,
	_game_engine_event_splitscreen_suppression_suppress_audio_and_text,

	k_game_engine_event_splitscreen_suppression_count
};

enum e_game_engine_sound_response_flags
{
	_game_engine_sound_response_announcer_sound_bit,

	k_game_engine_sound_response_flags
};

struct s_sound_response_extra_sounds_definition
{
	c_typed_tag_reference<SOUND_TAG> japanese_sound;
	c_typed_tag_reference<SOUND_TAG> german_sound;
	c_typed_tag_reference<SOUND_TAG> french_sound;
	c_typed_tag_reference<SOUND_TAG> spanish_sound;
	c_typed_tag_reference<SOUND_TAG> mexican_sound;
	c_typed_tag_reference<SOUND_TAG> italian_sound;
	c_typed_tag_reference<SOUND_TAG> korean_sound;

	// Note on Chinese audio
	// Although text is different between simplified and traditional Chinese, audio should be the same.
	// Cantonese? Mandarin? I don't know. I'm a gwailo.
	c_typed_tag_reference<SOUND_TAG> chinese_sound_traditional;
	c_typed_tag_reference<SOUND_TAG> chinese_sound_simplified;

	c_typed_tag_reference<SOUND_TAG> portuguese_sound;
	c_typed_tag_reference<SOUND_TAG> russian_sound;

	void update_reference_names();
};
static_assert(sizeof(s_sound_response_extra_sounds_definition) == 0xB0);

struct s_multiplayer_event_sound_response_definition
{
	c_flags<e_game_engine_sound_response_flags, short, k_game_engine_sound_response_flags> sound_flags;
	byte AGQD[0x2]; // pad
	c_typed_tag_reference<SOUND_TAG> english_sound;
	s_sound_response_extra_sounds_definition extra_sounds;
	real probability;

	void update_reference_names();
};
static_assert(sizeof(s_multiplayer_event_sound_response_definition) == 0xC8);

struct s_multiplayer_event_response_definition
{
	c_flags<e_game_engine_event_flags, short, k_game_engine_event_flags> flags;
	short runtime_event_type;
	c_string_id event_id;
	c_enum<e_game_engine_event_audience, short, _game_engine_event_audience_cause_player, k_game_engine_event_audience_count> audience;
	short display_priority;
	short sub_priority;
	c_enum<e_game_engine_event_response_context, short, _game_engine_event_response_context_self, k_game_engine_event_response_context_count> display_context;
	c_string_id display_string;
	c_string_id medal_award;
	short earned_wp;
	byte ASDF[0x2]; // pad
	real display_time; // seconds
	c_enum<e_game_engine_event_input, short, _game_engine_event_input_none, k_game_engine_event_input_count> required_field;
	c_enum<e_game_engine_event_input, short, _game_engine_event_input_none, k_game_engine_event_input_count> excluded_audience;
	c_enum<e_game_engine_event_splitscreen_suppression, short, _game_engine_event_splitscreen_suppression_none, k_game_engine_event_splitscreen_suppression_count> splitscreen_suppression;
	byte CRAP[0x2]; // pad
	c_string_id primary_string;
	long primary_string_duration; // seconds
	c_string_id plural_display_string;
	real sound_delay_announcer_only;

	// inlined `s_multiplayer_event_sound_response_definition`
	c_flags<e_game_engine_sound_response_flags, short, k_game_engine_sound_response_flags> sound_flags;
	byte PDHM[0x2]; // pad
	c_typed_tag_reference<SOUND_TAG> sound;
	s_sound_response_extra_sounds_definition extra_sounds;
	real probability;

	c_typed_tag_block<s_multiplayer_event_sound_response_definition> sound_permutations;

	void update_reference_names();
};
static_assert(sizeof(s_multiplayer_event_response_definition) == 0x10C);

