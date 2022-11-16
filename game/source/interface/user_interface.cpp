#include "interface/user_interface.hpp"

#include "cseries/cseries.hpp"
#include "interface/c_controller.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00A84C00, user_interface_scoreboard_update);

void __cdecl user_interface_update_console_scoreboard()
{
    c_gui_screen_scoreboard::update_scoreboard_alpha(k_any_controller);
    if (user_interface_should_show_console_scoreboard(0))
    {
        bool scoreboard_screen_is_above = false;
        c_gui_screen_scoreboard* scoreboard_screen = c_gui_screen_scoreboard::get_scoreboard_screen(k_any_controller);
        if (scoreboard_screen)
        {
            c_window_manager* window_manager = window_manager_get();
            if (window_manager->get_screen_above(_window_index4, scoreboard_screen))
                scoreboard_screen_is_above = true;
        }

        if (!scoreboard_screen_is_above)
            c_gui_screen_scoreboard::show_scoreboard(k_any_controller, false);
    }
    else
    {
        c_gui_screen_scoreboard::hide_scoreboard(k_any_controller);
    }
}

void __cdecl user_interface_scoreboard_update()
{
    user_interface_update_console_scoreboard();
    for (long i = 0; i < k_number_of_controllers; i++)
    {
        e_controller_index controller_index = static_cast<e_controller_index>(i);
    
        c_controller_interface* controller = controller_get(controller_index);
        if (!controller->in_use() && !user_interface_should_show_console_scoreboard(0))
            c_gui_screen_scoreboard::hide_scoreboard(controller_index);
    }
}

bool __cdecl user_interface_should_show_console_scoreboard(long* user_interface_show_busy_state)
{
    return INVOKE(0x00A84D60, user_interface_should_show_console_scoreboard, user_interface_show_busy_state);
}

