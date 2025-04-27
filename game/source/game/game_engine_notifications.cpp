#include "game/game_engine_notifications.hpp"

#include "cseries/cseries.hpp"

void __cdecl game_engine_player_deleted(int32 player_index)
{
	INVOKE(0x005A1460, game_engine_player_deleted, player_index);
}

