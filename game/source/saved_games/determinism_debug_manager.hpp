#pragma once

#include "cseries/cseries.hpp"

struct s_determinism_verification
{
	uns32 unchanged_field_mask;
	int32 game_state_check;
	int32 cheats_check;
	c_static_array<int32, 30> consumer_check;
};
COMPILE_ASSERT(sizeof(s_determinism_verification) == 0x84);

extern void __cdecl determinism_debug_manager_initialize();
extern void __cdecl determinism_debug_manager_dispose();
extern void __cdecl determinism_debug_manager_initialize_for_new_map();
extern void __cdecl determinism_debug_manager_dispose_from_old_map();

