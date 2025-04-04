#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"

#include "interface/c_gui_list_item_widget.hpp"
#include "interface/gui_selected_items.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_text.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "memory/module.hpp"
#include "saved_games/saved_game_files.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B21A20, c_gui_screen_pregame_lobby, handle_controller_input_message_);
HOOK_DECLARE_CLASS_MEMBER(0x00B21EA0, c_gui_screen_pregame_lobby, handle_list_item_chosen_);
HOOK_DECLARE_CLASS_MEMBER(0x00B22140, c_gui_screen_pregame_lobby, initialize_);

// $TODO: remove this once things are properly implemented for `advanced_options`
bool __thiscall c_gui_screen_pregame_lobby::handle_controller_input_message_(c_controller_input_message* message)
{
	if (message->get_event_type() == _event_type_button_press && message->get_component() == _controller_component_button_x)
	{
		// `c_gui_screen_pregame_lobby_multiplayer::handle_list_item_chosen`
		// `c_gui_screen_pregame_lobby_mapeditor::handle_list_item_chosen`
		load_game_variant_editing_screen(message->get_controller());
	}

	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_gui_screen_pregame_lobby, handle_controller_input_message_, message);
	return result;
}

bool c_gui_screen_pregame_lobby::handle_list_item_chosen_(c_controller_input_message const* message, long list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

void __thiscall c_gui_screen_pregame_lobby::initialize_()
{
	INVOKE_CLASS_MEMBER(0x00AB14D0, c_gui_screen_widget, initialize);

	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-title", this, parse_lobby_title));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-header", this, parse_lobby_header));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-network", this, parse_lobby_network));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-party-leader", this, parse_lobby_party_leader));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-start-button-name", this, parse_lobby_start_button_name));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-current-players", this, parse_lobby_current_players));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-max-players", this, parse_lobby_max_players));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-countdown-remaining", this, parse_lobby_countdown_remaining));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-privacy", this, parse_lobby_privacy));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-delaying-player", this, parse_lobby_delaying_player));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-film-max-players", this, parse_lobby_film_max_players));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-film-party-leader-requirement", this, parse_lobby_film_party_leader_requirement));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-coop-max-players", this, parse_lobby_coop_max_players));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-percent-loaded", this, parse_lobby_percent_loaded));
}

bool c_gui_screen_pregame_lobby::handle_list_item_chosen(c_controller_input_message const* message, long list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00B21EA0, c_gui_screen_pregame_lobby, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	if (list_name == STRING_ID(gui, lobby_list))
	{
		long target_name = _string_id_invalid;
		long element_handle = list_item_widget->get_element_handle();
		if (datasource->get_string_id_value(element_handle, STRING_ID(gui, target), &target_name))
		{
			if (target_name == STRING_ID(gui, network_mode))
			{
				if (c_load_pregame_selection_screen_message* screen_message = new c_load_pregame_selection_screen_message(
					message->get_controller(),
					get_render_window(),
					m_name.get_value(),
					_gui_selection_type_network_mode))
				{
					screen_message->set_parent_screen_index(m_screen_index);
					user_interface_messaging_post(screen_message);
				}
				return true;
			}
		}
		else if (datasource->get_string_id_value(element_handle, STRING_ID(global, name), &target_name))
		{
			switch (target_name)
			{
			case STRING_ID(gui, switch_lobby):
			{
				if (c_load_screen_message* screen_message = new c_load_screen_message(
					STRING_ID(gui, pregame_switch_lobby),
					k_any_controller,
					get_render_window(),
					m_name.get_value()))
				{
					screen_message->set_parent_screen_index(m_screen_index);
					screen_message->set_focus_on_load_by_name(
						STRING_ID(gui, switch_lobby),
						STRING_ID(global, name),
						user_interface_networking_get_name_from_gui_game_mode(get_gui_game_mode()));
					user_interface_messaging_post(screen_message);
				}
			}
			break;
			case STRING_ID(gui, start_game):
			{
				if (user_interface_squad_start_countdown_timer(message->get_controller(), 6, 4))
				{
					e_controller_index saved_game_controller = user_interface_networking_get_saved_game_controller();
					if (VALID_INDEX(saved_game_controller, k_number_of_controllers) &&
						user_interface_squad_get_player_count() == 1 &&
						saved_game_files_controller_has_saved_game_state_blocking(saved_game_controller))
					{
						user_interface_set_reload_from_persistent_storage(saved_game_controller);
					}
					else
					{
						user_interface_set_reload_from_persistent_storage(k_no_controller);
					}
				}
				else
				{
					play_sound(_ui_global_sound_effect_failure);
				}
			}
			break;
			}
			return true;
		}
	}
	else if (list_name == STRING_ID(gui, roster))
	{
		long target_session_player_index = NONE;
		long element_handle = list_item_widget->get_element_handle();
		if (datasource->get_integer_value(element_handle, STRING_ID(gui, player_index), &target_session_player_index))
		{
			if (s_player_identifier const* player_identifier = user_interface_session_get_player_identifier(target_session_player_index))
			{
				if (select_player_in_roster(message->get_controller(), player_identifier))
					return true;
			}
		}
	}

	return c_gui_screen_widget::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

void c_gui_screen_pregame_lobby::load_game_variant_editing_screen(e_controller_index controller_index)
{
	INVOKE_CLASS_MEMBER(0x00B225B0, c_gui_screen_pregame_lobby, load_game_variant_editing_screen, controller_index);
}

bool c_gui_screen_pregame_lobby::select_player_in_roster(e_controller_index controller_index, s_player_identifier const* target_player_identifier)
{
	return INVOKE_CLASS_MEMBER(0x00B22B80, c_gui_screen_pregame_lobby, select_player_in_roster, controller_index, target_player_identifier);
}

