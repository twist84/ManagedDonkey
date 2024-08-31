#pragma once

#include "cseries/cseries.hpp"

struct rumble_global_data
{
	byte __data[0x22C];
};
static_assert(sizeof(rumble_global_data) == 0x22C);

extern void __cdecl rumble_clear_all_now();
extern void __cdecl rumble_dispose();
extern void __cdecl rumble_dispose_from_old_map();
extern void __cdecl rumble_initialize();
extern void __cdecl rumble_initialize_for_new_map();
extern void __cdecl rumble_update(real world_seconds_elapsed);

