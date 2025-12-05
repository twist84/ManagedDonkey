#pragma once

#include "cseries/cseries.hpp"

struct s_player_training_globals
{
	byte __data[0x8E8];
};
static_assert(sizeof(s_player_training_globals) == 0x8E8);

extern void __cdecl player_training_dispose();
extern void __cdecl player_training_dispose_from_old_map();
extern void __cdecl player_training_initialize();
extern void __cdecl player_training_initialize_for_new_map();
extern void __cdecl player_training_notify_equipment_pickup(int32 player_index);
extern void __cdecl player_training_render_screen(int32 user_index);

