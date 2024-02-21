#include "game/game_grief.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x024B06B4, s_game_grief_globals, game_grief_globals);

void __cdecl game_grief_record_reprieve()
{
	INVOKE(0x00678F10, game_grief_record_reprieve);
}

bool __cdecl game_grief_can_eject(long player_index)
{
	return INVOKE(0x00678F30, game_grief_can_eject, player_index);
}

void __cdecl game_grief_dispose()
{
	INVOKE(0x00678F90, game_grief_dispose);
}

void __cdecl game_grief_dispose_from_old_map()
{
	INVOKE(0x00678FA0, game_grief_dispose_from_old_map);
}

bool __cdecl game_grief_get_ui_active_for_local_user(e_controller_index controller_index)
{
	return INVOKE(0x00678FB0, game_grief_get_ui_active_for_local_user, controller_index);
}

void __cdecl game_grief_initialize()
{
	INVOKE(0x00678FD0, game_grief_initialize);
}

void __cdecl game_grief_initialize_for_new_map()
{
	INVOKE(0x00678FF0, game_grief_initialize_for_new_map);
}

void __cdecl game_grief_record_betrayal(long player_index)
{
	INVOKE(0x00679010, game_grief_record_betrayal, player_index);
}

void __cdecl game_grief_record_ejection(long player_index)
{
	INVOKE(0x00679070, game_grief_record_ejection, player_index);
}

void __cdecl sub_6790A0(e_controller_index controller_index, bool a2)
{
	INVOKE(0x006790A0, sub_6790A0, controller_index, a2);
}

void __cdecl game_grief_update()
{
	INVOKE(0x006790D0, game_grief_update);
}

