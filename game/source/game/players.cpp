#include "players.hpp"

#include "cseries/console.hpp"

long __cdecl players_first_active_user()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00589A30, players_first_active_user);
}

bool __cdecl player_teleport(long player_index, long object_index, real_point3d const* position)
{
	return INVOKE(0x0053F550, player_teleport, player_index, object_index, position);
}
