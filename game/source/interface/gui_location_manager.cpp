#include "interface/gui_location_manager.hpp"

#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "main/main_game.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/online/online_files.hpp"
#include "networking/session/network_session.hpp"
#include "tag_files/string_ids.hpp"

REFERENCE_DECLARE(0x01944204, c_gui_location_manager, g_location_manager);

HOOK_DECLARE_CLASS_MEMBER(0x00ADFA10, c_gui_location_manager, update_);
HOOK_DECLARE(0x00ADF9D0, location_manager_get);
HOOK_DECLARE(0x00ADF9E0, location_manager_start);
HOOK_DECLARE(0x00ADF9F0, location_manager_stop);

const int32 k_location_change_throttle_threshold = 30;

void __thiscall c_gui_location_manager::update_()
{
	c_gui_location_manager::update();
}

c_gui_location_manager::c_gui_location_manager() :
	m_running(true),
	m_location_mode(_location_mode_unset),
	m_show_postgame_stats(false),
	m_location_change_throttle_time(-1),
	m_location_change_throttle_count(k_location_change_throttle_threshold)
{
	//DECLFUNC(0x00ADF680, void, __thiscall, c_gui_location_manager*)(this);
}

void __cdecl c_gui_location_manager::begin_enter_location(e_gui_location old_location, e_gui_location new_location)
{
	//INVOKE(0x00ADF6A0, c_gui_location_manager::begin_enter_location, old_location, new_location);

	switch (new_location)
	{
	case _gui_location_main_menu:
	case _gui_location_pregame_lobby:
	{
		c_online_file_manager::get()->start();
	}
	break;
	case _gui_location_matchmaking_searching:
	case _gui_location_matchmaking_match_found:
	case _gui_location_in_game:
	case _gui_location_postgame_lobby:
	{
		c_online_file_manager::get()->stop();
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}
}

bool c_gui_location_manager::can_change_location()
{
	//return INVOKE_CLASS_MEMBER(0x00ADF6C0, c_gui_location_manager, can_change_location);

	return m_location_change_throttle_count >= k_location_change_throttle_threshold;
}

void c_gui_location_manager::change_location(int32 new_location_name)
{
	//INVOKE_CLASS_MEMBER(0x00ADF6E0, c_gui_location_manager, change_location, new_location_name);

	if (!c_gui_location_manager::can_change_location())
	{
		return;
	}
	m_location_change_throttle_count = 0;

	if (new_location_name == _string_id_invalid)
	{
		return;
	}

	e_screen_transition_type transition_type = _screen_transition_type_normal;
	if (c_gui_screen_widget* location_screen = c_gui_location_manager::get_location_screen())
	{
		bool goto_matchmaking_searching = new_location_name == STRING_ID(gui, matchmaking_searching) && location_screen->m_name == STRING_ID(gui, pregame_lobby_matchmaking);
		bool goto_pregame_lobby_matchmaking = new_location_name == STRING_ID(gui, pregame_lobby_matchmaking) && location_screen->m_name == STRING_ID(gui, matchmaking_searching);
		if (goto_matchmaking_searching || goto_pregame_lobby_matchmaking)
		{
			transition_type = _screen_transition_type_custom0;
		}
		location_screen->transition_out_with_transition_type(_transition_out_normal, transition_type);
	}

	const c_gui_screen_widget* exempt_screens[7]{};
	int32 except_these_count = 6;
	if (new_location_name == STRING_ID(gui, matchmaking_searching))
	{
		except_these_count = 7;
		exempt_screens[0] = window_manager_get()->get_screen_by_name(k_number_of_player_windows, STRING_ID(gui, carnage_report));
	}

	exempt_screens[1] = window_manager_get()->get_screen_by_name(k_number_of_player_windows, STRING_ID(gui, spartan_milestone_dialog));
	exempt_screens[2] = window_manager_get()->get_screen_by_name(k_number_of_player_windows, STRING_ID(gui, spartan_rank_dialog));
	exempt_screens[3] = window_manager_get()->get_screen_by_name(k_number_of_player_windows, STRING_ID(gui, gui_alert_toast));
	exempt_screens[4] = window_manager_get()->get_screen_by_name(k_number_of_player_windows, STRING_ID(gui, gui_alert_nonblocking));
	exempt_screens[5] = window_manager_get()->get_screen_by_name(k_number_of_player_windows, STRING_ID(gui, gui_alert_ingame_full));
	exempt_screens[6] = window_manager_get()->get_screen_by_name(k_number_of_player_windows, STRING_ID(gui, gui_alert_ingame_split));
	window_manager_get()->close_all_screens(exempt_screens + (except_these_count == 7), except_these_count);

	c_load_screen_message* screen_message = new (_ui_allocation_marker_dummy) c_load_screen_message(
		new_location_name,
		k_any_controller,
		k_number_of_player_windows,
		STRING_ID(gui, bottom_most));
	if (screen_message)
	{
		user_interface_messaging_post(screen_message);
		screen_message->set_transition_type(transition_type);
	}
}

e_gui_location c_gui_location_manager::get_current_ui_location()
{
	//return INVOKE_CLASS_MEMBER(0x00ADF870, c_gui_location_manager, get_current_ui_location);

	if (game_in_progress() && !game_is_ui_shell())
	{
		return _gui_location_in_game;
	}

	c_gui_screen_widget* location_screen = c_gui_location_manager::get_location_screen();
	if (!location_screen)
	{
		return _gui_location_none;
	}

	return location_screen->get_gui_location();
}

c_gui_screen_widget* c_gui_location_manager::get_location_screen()
{
	//return INVOKE_CLASS_MEMBER(0x00ADF8A0, c_gui_location_manager, get_location_screen);

	for (c_gui_screen_widget* location_screen = window_manager_get()->get_topmost_screen(k_number_of_player_windows);
		location_screen;
		location_screen = window_manager_get()->get_screen_below(k_number_of_player_windows, location_screen))
	{
		if (location_screen->transitioning_out() || location_screen->get_gui_location() == _gui_location_none)
		{
			continue;
		}

		return location_screen;
	}

	return nullptr;
}

int32 c_gui_location_manager::get_location_screen_name(int32 new_location)
{
	//return INVOKE_CLASS_MEMBER(0x00ADF8F0, c_gui_location_manager, get_location_screen_name, new_location);

	int32 result = _string_id_invalid;
	switch (new_location)
	{
	case _gui_location_main_menu:
	{
		result = STRING_ID(gui, main_menu);
	}
	break;
	case _gui_location_pregame_lobby:
	{
		result = c_gui_location_manager::get_pregame_lobby_name(user_interface_squad_get_ui_game_mode());
	}
	break;
	case _gui_location_matchmaking_searching:
	{
		result = STRING_ID(gui, matchmaking_searching);
	}
	break;
	case _gui_location_matchmaking_match_found:
	{
		result = STRING_ID(gui, matchmaking_match_found);
	}
	break;
	case _gui_location_in_game:
	{
		result = _string_id_invalid;
	}
	break;
	case _gui_location_postgame_lobby:
	{
		result = STRING_ID(gui, postgame_lobby);
	}
	break;
	}
	return result;
}

int32 c_gui_location_manager::get_pregame_lobby_name(int32 game_mode)
{
	//return INVOKE_CLASS_MEMBER(0x00ADF960, c_gui_location_manager, get_pregame_lobby_name, game_mode);

	int32 result = _string_id_invalid;
	switch (game_mode)
	{
	case _gui_game_setup_mode_none:
	{
		result = _string_id_invalid;
	}
	break;
	case _gui_game_setup_mode_campaign:
	{
		result = STRING_ID(gui, pregame_lobby_campaign);
	}
	break;
	case _gui_game_setup_mode_matchmaking:
	{
		result = STRING_ID(gui, pregame_lobby_matchmaking);
	}
	break;
	case _gui_game_setup_mode_multiplayer:
	{
		result = STRING_ID(gui, pregame_lobby_multiplayer);
	}
	break;
	case _gui_game_setup_mode_mapeditor:
	{
		result = STRING_ID(gui, pregame_lobby_mapeditor);
	}
	break;
	case _gui_game_setup_mode_theater:
	{
		result = STRING_ID(gui, pregame_lobby_theater);
	}
	break;
	//case _gui_game_setup_mode_survival:
	//{
	//	result = STRING_ID(gui, pregame_lobby_survival);
	//}
	//break;
	}
	return result;
}

bool c_gui_location_manager::get_show_postgame_stats_upon_lobby_entrance() const
{
	return m_show_postgame_stats;
}

c_gui_location_manager* __cdecl location_manager_get()
{
	//return INVOKE(0x00ADF9D0, location_manager_get);

	return &g_location_manager;
}

void __cdecl location_manager_start()
{
	//INVOKE(0x00ADF9E0, location_manager_start);

	g_location_manager.m_running = true;
}

void __cdecl location_manager_stop()
{
	//INVOKE(0x00ADF9F0, location_manager_stop);

	g_location_manager.m_running = false;
}

void c_gui_location_manager::set_running(bool running)
{
	//INVOKE_CLASS_MEMBER(0x00ADFA00, c_gui_location_manager, set_running, running);

	m_running = running;
}

void c_gui_location_manager::set_show_postgame_stats_upon_lobby_entrance(bool value)
{
	m_show_postgame_stats = value;
}

void c_gui_location_manager::update()
{
	//INVOKE_CLASS_MEMBER(0x00ADFA10, c_gui_location_manager, update);

	if (m_running)
	{
		update_change_location();

		e_gui_location current_networking_location = user_interface_networking_get_current_location();
		if (current_networking_location != _gui_location_none && !user_interface_networking_get_start_game_when_ready())
		{
			if (m_location_mode == _location_mode_unset || current_networking_location)
			{
				m_location_mode = e_location_mode(_location_mode_editor_or_game_start - user_interface_squad_exists());
			}

			e_gui_location current_ui_location = c_gui_location_manager::get_current_ui_location();
			c_gui_screen_widget* location_screen = c_gui_location_manager::get_location_screen();

			c_network_session* session = nullptr;
			bool in_session = network_life_cycle_in_session(&session);
			if (game_in_progress() &&
				game_is_ui_shell() &&
				current_networking_location == _gui_location_in_game &&
				!network_life_cycle_map_load_pending() &&
				(!in_session || session->is_host()))
			{
				event(_event_warning, "ui:location_manager: Resetting network location.  If you got here and didn't just issue a console command, this is a bug.");
				network_life_cycle_end();
			}
			else if (current_ui_location == current_networking_location)
			{
				if (current_ui_location == _gui_location_pregame_lobby)
				{
					e_gui_game_mode game_mode = user_interface_squad_get_ui_game_mode();
					if (game_mode != _gui_game_setup_mode_none)
					{
						ASSERT(location_screen && location_screen->get_gui_location() == _gui_location_pregame_lobby);

						c_gui_screen_pregame_lobby* pregame_lobby_screen = (c_gui_screen_pregame_lobby*)location_screen;
						if (pregame_lobby_screen->get_gui_game_mode() != game_mode)
						{
							c_gui_location_manager::change_location(c_gui_location_manager::get_pregame_lobby_name(game_mode));
						}
					}
				}
			}
			else if (current_networking_location || current_ui_location != _gui_location_in_game || m_location_mode != _location_mode_editor_or_game_start)
			{
				if (!game_is_ui_shell() && (current_ui_location == _gui_location_in_game || current_networking_location == _gui_location_pregame_lobby))
				{
					main_menu_launch();
					c_gui_location_manager::set_show_postgame_stats_upon_lobby_entrance(true);
				}
				else
				{
					c_gui_location_manager::begin_enter_location(current_ui_location, current_networking_location);
					c_gui_location_manager::change_location(c_gui_location_manager::get_location_screen_name(current_networking_location));
				}
			}

			if (location_screen != nullptr)
			{
				for (c_gui_screen_widget* screen_widget = window_manager_get()->get_screen_below(k_number_of_player_windows, location_screen);
					screen_widget;
					screen_widget = window_manager_get()->get_screen_below(k_number_of_player_windows, screen_widget))
				{
					if (screen_widget->transitioning_out())
					{
						break;
					}

					screen_widget->transition_out(_transition_out_normal);
				}
			}
		}
	}
}

void c_gui_location_manager::update_change_location()
{
	uns32 current_ui_time = user_interface_milliseconds();
	if (current_ui_time != m_location_change_throttle_time)
	{
		if (!can_change_location())
		{
			m_location_change_throttle_count++;
		}
	}
	m_location_change_throttle_time = current_ui_time;
}

