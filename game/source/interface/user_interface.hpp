#pragma once

#include "cseries/cseries.hpp"

extern void __cdecl user_interface_scoreboard_update();
extern bool __cdecl user_interface_should_show_console_scoreboard(long* user_interface_show_busy_state);
extern void __cdecl user_interface_update(real ui_time);

