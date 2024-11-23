#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"

#include "interface/c_controller.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_text.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B21A20, c_gui_screen_pregame_lobby, handle_controller_input_message_);
HOOK_DECLARE_CLASS_MEMBER(0x00B22140, c_gui_screen_pregame_lobby, initialize_);

bool __thiscall c_gui_screen_pregame_lobby::handle_controller_input_message_(c_controller_input_message* message)
{
	if (message->get_event_type() == _event_type_controller_component && message->get_component() == _controller_component_button_x)
	{
		e_controller_index controller_index = message->get_controller();

		// `c_gui_screen_pregame_lobby_multiplayer::handle_list_item_chosen`
		// `c_gui_screen_pregame_lobby_mapeditor::handle_list_item_chosen`
		// if (gui_data->get_string_id_value(list_item->get_element_handle(), STRING_ID(global, name), name) && name == STRING_ID(gui, advanced_options))
		DECLFUNC(0x00B225B0, void, __thiscall, void*, e_controller_index)(this, controller_index);
	}

	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_gui_screen_pregame_lobby, handle_controller_input_message_, message);
	return result;
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

