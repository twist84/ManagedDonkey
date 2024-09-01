#include "game/game_progression.hpp"

long __cdecl game_progression_get_last_level()
{
	return INVOKE(0x006715F0, game_progression_get_last_level);
}

bool __cdecl game_progression_level_has_gameplay(long level_index)
{
	return INVOKE(0x006716F0, game_progression_level_has_gameplay, level_index);
}

bool __cdecl game_progression_level_is_hub(long level_index)
{
	return INVOKE(0x00671810, game_progression_level_is_hub, level_index);
}

