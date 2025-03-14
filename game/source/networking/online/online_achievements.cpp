#include "networking/online/online_achievements.hpp"

void __cdecl achievements_dispose()
{
	INVOKE(0x014E22D0, achievements_dispose);
}

void __cdecl achievements_initialize()
{
	INVOKE(0x014E2320, achievements_initialize);
}

void __cdecl achievements_update(real world_seconds_elapsed)
{
	INVOKE(0x014E2440, achievements_update, world_seconds_elapsed);
}

void __cdecl gamer_achievements_begin_retrieval(e_controller_index controller_index)
{
	INVOKE(0x014E2620, gamer_achievements_begin_retrieval, controller_index);
}

