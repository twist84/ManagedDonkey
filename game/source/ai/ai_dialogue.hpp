#pragma once

#include "cseries/cseries.hpp"

struct vocalization_records :
	s_datum_header
{
	byte __data[0x5A];
};
static_assert(sizeof(vocalization_records) == 0x5C);

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

