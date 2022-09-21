#include "players.hpp"

#include "cseries/console.hpp"

long __cdecl players_first_active_user()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00589A30, players_first_active_user);
}