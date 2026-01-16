#pragma once

#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
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

struct s_multiplayer_event_sound_response_definition
{
	c_flags<e_game_engine_sound_response_flags, uns16, k_game_engine_sound_response_flags> flags;
	uns16 pad;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> sounds[k_language_count];
	real32 probability;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_event_sound_response_definition) == 0xC8);

struct s_multiplayer_event_response_definition
{
	c_flags<e_game_engine_event_flags, int16, k_game_engine_event_flags> flags;
	int16 runtime_event_type;
	c_string_id event_id;
	c_enum<e_game_engine_event_audience, int16, _game_engine_event_audience_cause_player, k_game_engine_event_audience_count> audience;
	int16 display_priority;
	int16 sub_priority;
	c_enum<e_game_engine_event_response_context, int16, _game_engine_event_response_context_self, k_game_engine_event_response_context_count> display_context;
	c_string_id display_string;
	c_string_id medal_award;
	int16 earned_wp;
	uns16 pad;
	real32 display_time; // seconds
	c_enum<e_game_engine_event_input, int16, _game_engine_event_input_none, k_game_engine_event_input_count> required_field;
	c_enum<e_game_engine_event_input, int16, _game_engine_event_input_none, k_game_engine_event_input_count> excluded_audience;
	c_enum<e_game_engine_event_splitscreen_suppression, int16, _game_engine_event_splitscreen_suppression_none, k_game_engine_event_splitscreen_suppression_count> splitscreen_suppression;
	int16 pad2;
	c_string_id primary_string;
	int32 primary_string_duration; // seconds
	c_string_id plural_display_string;
	real32 sound_delay_announcer_only;
	s_multiplayer_event_sound_response_definition default_sound;
	c_typed_tag_block<s_multiplayer_event_sound_response_definition> sound_permutations;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_event_response_definition) == 0x10C);

