#pragma once

#include "cseries/cseries.hpp"

struct s_game_allegiance_globals
{
	byte __data[0x184];
};
static_assert(sizeof(s_game_allegiance_globals) == 0x184);

extern bool __cdecl game_allegiance_betrayal_exists();
extern void __cdecl game_allegiance_dispose();
extern void __cdecl game_allegiance_dispose_from_old_map();
extern void __cdecl game_allegiance_initialize();
extern void __cdecl game_allegiance_initialize_for_new_map();
extern void __cdecl game_allegiance_update();

