#pragma once

#include "cseries/cseries.hpp"

struct s_determinism_verification
{
	uint32 unchanged_field_mask;
	long game_state_check;
	long cheats_check;
	c_static_array<long, 30> consumer_check;
};
static_assert(sizeof(s_determinism_verification) == 0x84);

extern void __cdecl determinism_debug_manager_initialize();
extern void __cdecl determinism_debug_manager_dispose();
extern void __cdecl determinism_debug_manager_initialize_for_new_map();
extern void __cdecl determinism_debug_manager_dispose_from_old_map();

