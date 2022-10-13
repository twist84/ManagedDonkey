#include "game/game_engine.hpp"

#include "cseries/console.hpp"
#include "game/game_time.hpp"
#include "memory/module.hpp"

//HOOK_DECLARE(0x005521D0, game_engine_get_pre_round_ticks);

long __cdecl game_engine_get_pre_round_ticks()
{
	FUNCTION_BEGIN(false);

    return game_seconds_integer_to_ticks(8);
}
