#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE_ARRAY(0x05269788, real, c_gui_screen_scoreboard::m_scoreboard_alpha, 4);
REFERENCE_DECLARE(0x05269798, real, c_gui_screen_scoreboard::m_console_scoreboard_alpha);

real __cdecl c_gui_screen_scoreboard::get_scoreboard_alpha(e_controller_index controller_index)
{
    return INVOKE(0x00AB30F0, c_gui_screen_scoreboard::get_scoreboard_alpha, controller_index);
}

c_gui_screen_scoreboard* __cdecl c_gui_screen_scoreboard::get_scoreboard_screen(e_controller_index controller_index)
{
    return INVOKE(0x00AB3120, c_gui_screen_scoreboard::get_scoreboard_screen, controller_index);
}

void __cdecl c_gui_screen_scoreboard::hide_scoreboard(e_controller_index controller_index)
{
    return INVOKE(0x00AB3560, c_gui_screen_scoreboard::hide_scoreboard, controller_index);
}

bool __cdecl c_gui_screen_scoreboard::is_scoreboard_displayed(e_controller_index controller_index)
{
    return INVOKE(0x00AB3690, c_gui_screen_scoreboard::is_scoreboard_displayed, controller_index);
}

bool __cdecl c_gui_screen_scoreboard::is_scoreboard_interactive(e_controller_index controller_index)
{
    return INVOKE(0x00AB3740, c_gui_screen_scoreboard::is_scoreboard_interactive, controller_index);
}

void __cdecl c_gui_screen_scoreboard::show_scoreboard(e_controller_index controller_index, bool is_interactive)
{
    INVOKE(0x00AB3C60, c_gui_screen_scoreboard::show_scoreboard, controller_index, is_interactive);
}

void __cdecl c_gui_screen_scoreboard::update_scoreboard_alpha(e_controller_index controller_index)
{
    INVOKE(0x00AB4C50, c_gui_screen_scoreboard::update_scoreboard_alpha, controller_index);
}

