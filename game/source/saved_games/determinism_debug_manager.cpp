#include "saved_games/determinism_debug_manager.hpp"

void __cdecl determinism_debug_manager_initialize()
{
	INVOKE(0x00530270, determinism_debug_manager_initialize);
}

void __cdecl determinism_debug_manager_dispose()
{
	INVOKE(0x00530280, determinism_debug_manager_dispose);
}

void __cdecl determinism_debug_manager_initialize_for_new_map()
{
	INVOKE(0x00530290, determinism_debug_manager_initialize_for_new_map);
}

void __cdecl determinism_debug_manager_dispose_from_old_map()
{
	INVOKE(0x005302A0, determinism_debug_manager_dispose_from_old_map);
}

