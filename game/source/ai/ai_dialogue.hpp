#pragma once

#include "cseries/cseries.hpp"
#include "ai/event_handling.hpp"

enum e_dialogue_state
{
	_prop_dialogue_state_none = 0,
	_prop_dialogue_state_unacknowledged,
	_prop_dialogue_state_never_seen,
	_prop_dialogue_state_acknowledged,
	_prop_dialogue_state_hidden,
	_prop_dialogue_state_abandoned,
	_prop_dialogue_state_dead,

	k_prop_dialogue_state_count,
};

const struct s_game_sound_deterministic_permutation
{
	c_enum<int32, int8, -1, 12> pitch_range_index;
	c_enum<int32, int8, -1, 32> permutation_index;
};

struct s_vocalization_record :
	s_datum_header
{
	int16 vocalization_index;
	int32 speaker_unit_index;
	int32 sound_definition_index;
	int16 dialogue_team_index;
	bool same_team_response;
	int32 creation_time;
	int32 start_time;
	int16 notification_delay;
	int16 post_delay;
	int16 duration;
	int16 allowable_queue_delay;
	s_game_sound_deterministic_permutation deterministic_permutation;
	int16 priority;
	e_dialogue_state required_dialogue_state;
	ai_information_packet ai_packet;
	bool started;
	bool notified;
	int32 responding_to_record_index;
	bool remove;
	int32 variant_designator;
	int32 next_record_index;
};
static_assert(sizeof(s_vocalization_record) == 0x5C);

struct vocalization_timers
{
	byte __data[0xFB8];
};
static_assert(sizeof(vocalization_timers) == 0xFB8);

extern void __cdecl ai_dialogue_dispose_from_old_map();
extern void __cdecl ai_dialogue_initialize();
extern void __cdecl ai_dialogue_initialize_for_new_map();
extern void __cdecl ai_dialogue_update();
extern real64 __cdecl dialogue_player_weighting(int32 unit_index);
extern int16 __cdecl dialogue_team_get(int16 team);

extern void ai_dialogue_render_player_weights();

