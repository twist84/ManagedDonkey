#include "game/game_progression.hpp"

int32 __cdecl game_progression_get_last_level()
{
	return INVOKE(0x006715F0, game_progression_get_last_level);
}

bool __cdecl game_progression_level_has_gameplay(int32 level_index)
{
	return INVOKE(0x006716F0, game_progression_level_has_gameplay, level_index);
}

bool __cdecl game_progression_level_is_hub(int32 level_index)
{
	return INVOKE(0x00671810, game_progression_level_is_hub, level_index);
}

