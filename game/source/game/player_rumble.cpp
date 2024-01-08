#include "game/player_rumble.hpp"

void __cdecl rumble_clear_all_now()
{
	INVOKE(0x006155D0, rumble_clear_all_now);
}

void __cdecl rumble_update(real world_seconds_elapsed)
{
	INVOKE(0x00615BA0, rumble_update, world_seconds_elapsed);
}

