#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

extern long __cdecl user_interface_squad_get_countdown_delaying_player();
extern long __cdecl user_interface_squad_get_countdown_timer();
extern e_session_game_start_status __cdecl user_interface_get_session_game_start_status(e_session_game_start_error* error, dword* player_error_mask);
extern bool __cdecl user_interface_squad_is_player_valid(long session_player_index);

