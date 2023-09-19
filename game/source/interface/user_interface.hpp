#pragma once

#include "cseries/cseries.hpp"

extern real g_ui_time_scale;
extern real g_ui_time_step;

extern bool __cdecl get_alpha_custom_games_disabled();
extern bool __cdecl get_alpha_eula_accepted();
extern bool __cdecl get_alpha_is_external_beta();
extern bool __cdecl get_alpha_is_internal_beta();
extern bool __cdecl get_alpha_is_locked_down();
extern long __cdecl get_alpha_locked_down_state();
extern bool __cdecl get_is_alpha_version();
extern void __cdecl user_interface_scoreboard_update();
extern bool __cdecl user_interface_should_show_console_scoreboard(long* user_interface_show_busy_state);
extern void __cdecl user_interface_update(real ui_time);

