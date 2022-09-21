#include "game/game_state.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"

void __cdecl game_state_prepare_for_revert()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00510040, game_state_prepare_for_revert);
}