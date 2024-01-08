#include "networking/online/online_achievements.hpp"

void __cdecl achievements_update(real world_seconds_elapsed)
{
	INVOKE(0x014E2440, achievements_update, world_seconds_elapsed);
}

