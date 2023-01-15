#include "game/game_state.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x02344148, s_game_state_globals, game_state_globals);

void __cdecl game_state_prepare_for_revert()
{
	INVOKE(0x00510040, game_state_prepare_for_revert);
}

