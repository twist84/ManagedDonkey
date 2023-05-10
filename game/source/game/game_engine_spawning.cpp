#include "game/game_engine_spawning.hpp"

#include "game/game.hpp"
#include "game/game_engine_util.hpp"
#include "memory/data.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"

HOOK_DECLARE(0x005A41D0, game_engine_should_spawn_player);

bool __cdecl game_engine_should_spawn_player(long player_index)
{
	ASSERT(game_is_authoritative());
	
	if (!current_game_engine())
		return true;
	
	if (!game_engine_player_is_playing(player_index))
		return false;
	
	if (!game_engine_in_round())
		return false;

	TLS_REFERENCE(player_data);

	player_datum* player = (player_datum*)datum_try_and_get(player_data, player_index);

	if ((game_is_survival() || game_is_multiplayer()) && (game_engine_round_time_get() + player->respawn_timer) < game_engine_get_pre_round_ticks())
	{
		game_engine_player_set_spawn_timer(player_index, game_engine_get_pre_round_ticks() - game_engine_round_time_get());
		return false;
	}
	
	return player->respawn_forced || player->respawn_timer <= 0;
}

