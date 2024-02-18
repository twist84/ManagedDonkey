#include "interface/user_interface.hpp"

#include "cseries/cseries.hpp"
#include "interface/c_controller.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "memory/module.hpp"

real g_ui_time_scale = 1.0f;
real g_ui_time_step = 0.0f;

REFERENCE_DECLARE(0x052559E4, s_user_interface_globals, g_user_interface_globals);

HOOK_DECLARE(0x00A84C00, user_interface_scoreboard_update);

bool __cdecl get_alpha_custom_games_disabled()
{
	return INVOKE(0x00A84050, get_alpha_custom_games_disabled);
}

bool __cdecl get_alpha_eula_accepted()
{
	return INVOKE(0x00A840C0, get_alpha_eula_accepted);
}

bool __cdecl get_alpha_is_external_beta()
{
	return INVOKE(0x00A840E0, get_alpha_is_external_beta);
}

bool __cdecl get_alpha_is_internal_beta()
{
	return INVOKE(0x00A84120, get_alpha_is_internal_beta);
}

bool __cdecl get_alpha_is_locked_down()
{
	return INVOKE(0x00A84160, get_alpha_is_locked_down);
}

//e_alpha_configuration_ui_level __cdecl get_alpha_locked_down_state()
long __cdecl get_alpha_locked_down_state()
{
	return INVOKE(0x00A84190, get_alpha_locked_down_state);
}

bool __cdecl get_is_alpha_version()
{
	return INVOKE(0x00A841E0, get_is_alpha_version);
}

void __cdecl user_interface_enter_game_shell()
{
	INVOKE(0x00A84330, user_interface_enter_game_shell);

	//g_user_interface_globals.milliseconds_at_last_event = g_user_interface_globals.milliseconds.peek();
}

dword __cdecl user_interface_milliseconds()
{
	return INVOKE(0x00A848E0, user_interface_milliseconds);
}

bool __cdecl user_interface_requests_unlocked_framerate()
{
	return INVOKE(0x00A84BF0, user_interface_requests_unlocked_framerate);
}

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
	//INVOKE(0x00A84C00, user_interface_scoreboard_update);

	user_interface_update_console_scoreboard();
	for (long i = 0; i < k_number_of_controllers; i++)
	{
		e_controller_index controller_index = static_cast<e_controller_index>(i);

		c_controller_interface* controller = controller_get(controller_index);
		if (!controller->in_use() && !user_interface_should_show_console_scoreboard(0))
			c_gui_screen_scoreboard::hide_scoreboard(controller_index);
	}
}

s_user_interface_shared_globals const* user_interface_shared_tag_globals_try_and_get()
{
	return INVOKE(0x00A84D20, user_interface_shared_tag_globals_try_and_get);
}

bool __cdecl user_interface_should_show_console_scoreboard(long* user_interface_show_busy_state)
{
	return INVOKE(0x00A84D60, user_interface_should_show_console_scoreboard, user_interface_show_busy_state);
}

s_user_interface_tag_globals const* __cdecl user_interface_tag_globals_try_and_get()
{
	return INVOKE(0x00A84E80, user_interface_tag_globals_try_and_get);
}

void __cdecl user_interface_update(real ui_time)
{
	real new_ui_time = (ui_time * g_ui_time_scale) + g_ui_time_step;

	INVOKE(0x00A84EE0, user_interface_update, new_ui_time);
}
HOOK_DECLARE_CALL(0x00505DB1, user_interface_update);
//HOOK_DECLARE_CALL(0x0069D618, user_interface_update);
//HOOK_DECLARE_CALL(0x0069D648, user_interface_update);

