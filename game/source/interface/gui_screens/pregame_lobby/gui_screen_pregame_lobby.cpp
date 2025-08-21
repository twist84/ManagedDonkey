#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"

#include "cseries/cseries_events.hpp"
#include "game/game_engine_team.hpp"
#include "game/game_engine_variant.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/gui_location_manager.hpp"
#include "interface/gui_roster_data.hpp"
#include "interface/gui_roster_list_widget.hpp"
#include "interface/gui_selected_items.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_text.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "memory/module.hpp"
#include "networking/online/online.hpp"
#include "saved_games/saved_game_files.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B21A20, c_gui_screen_pregame_lobby, handle_controller_input_message_);
HOOK_DECLARE_CLASS_MEMBER(0x00B21EA0, c_gui_screen_pregame_lobby, handle_list_item_chosen_);
HOOK_DECLARE_CLASS_MEMBER(0x00B22140, c_gui_screen_pregame_lobby, initialize_);
HOOK_DECLARE_CLASS_MEMBER(0x00B224D0, c_gui_screen_pregame_lobby, initialize_datasource_);
HOOK_DECLARE_CLASS_MEMBER(0x00B22AF0, c_gui_screen_pregame_lobby, post_initialize_);
HOOK_DECLARE_CLASS_MEMBER(0x00B23440, c_gui_screen_pregame_lobby, update_widget_visiblility_);

// $TODO: remove this once things are properly implemented for `advanced_options`
bool __thiscall c_gui_screen_pregame_lobby::handle_controller_input_message_(const c_controller_input_message* message)
{
	if (message->get_event_type() == _event_type_button_press && message->get_component() == _controller_component_button_x)
	{
		// `c_gui_screen_pregame_lobby_multiplayer::handle_list_item_chosen`
		// `c_gui_screen_pregame_lobby_mapeditor::handle_list_item_chosen`
		c_gui_screen_pregame_lobby::load_game_variant_editing_screen(message->get_controller());
		return true;
	}

	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_gui_screen_pregame_lobby, handle_controller_input_message_, message);
	return result;
}

bool __thiscall c_gui_screen_pregame_lobby::handle_list_item_chosen_(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return c_gui_screen_pregame_lobby::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

void __thiscall c_gui_screen_pregame_lobby::initialize_()
{
	c_gui_screen_pregame_lobby::initialize();
}

void __thiscall c_gui_screen_pregame_lobby::initialize_datasource_()
{
	c_gui_screen_pregame_lobby::initialize_datasource();
}

void __thiscall c_gui_screen_pregame_lobby::post_initialize_()
{
	c_gui_screen_pregame_lobby::post_initialize();
}

void c_gui_screen_pregame_lobby::update_widget_visiblility_()
{
	c_gui_screen_pregame_lobby::update_widget_visiblility();
}

template<>
void ui_track_delete<c_gui_active_roster_data>(const c_gui_active_roster_data* object)
{
	ASSERT(object != NULL);

	object->~c_gui_active_roster_data();
	user_interface_free(object);
}

c_gui_screen_pregame_lobby::c_gui_screen_pregame_lobby(int32 name) :
	c_gui_screen_widget(name),
	m_last_countdown_value(NONE),
	m_is_fading(false),
	m_change_teams_visible(false),
	m_postgame_stats_visible(false),
	m_vidmaster_popup_downloaded(false),
	m_vidmaster_popup_downloader(),
	m_vidmaster_popup_shown(false),
	m_advanced_options_visible(false),
	m_url_key(0)
{
	//DECLFUNC(0x00B21180, void, __thiscall, c_gui_screen_pregame_lobby*, int32)(this, name);

	DECLFUNC(0x00AE7120, void, __thiscall, c_http_blf_simple_downloader<s_message_of_the_day_popup, 4665>*)(&m_vidmaster_popup_downloader);
}

//.text:00B21230 ; public: virtual void* c_gui_screen_pregame_lobby::`vector deleting destructor'(unsigned int)
c_gui_screen_pregame_lobby::~c_gui_screen_pregame_lobby()
{
	//DECLFUNC(0x00B21230, void, __thiscall, c_gui_screen_pregame_lobby*)(this);
}

bool c_gui_screen_pregame_lobby::advanced_options_enabled()
{
	bool result = false;
	switch (m_name)
	{
	case STRING_ID(gui, pregame_lobby_campaign):
	case STRING_ID(gui, pregame_lobby_matchmaking):
	case STRING_ID(gui, pregame_lobby_multiplayer):
	case STRING_ID(gui, pregame_lobby_mapeditor):
	{
		result = true;
	}
	break;
	}

	return result;
}

void c_gui_screen_pregame_lobby::commit_team_change(int32 player_index, c_gui_roster_list_widget* roster_list_widget, int32 lying_end_time)
{
	INVOKE_CLASS_MEMBER(0x00B212A0, c_gui_screen_pregame_lobby, commit_team_change, player_index, roster_list_widget, lying_end_time);
}

c_gui_bitmap_widget* c_gui_screen_pregame_lobby::create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition)
{
	return INVOKE_CLASS_MEMBER(0x00B212F0, c_gui_screen_pregame_lobby, create_bitmap_widget, definition);
}

c_gui_list_widget* c_gui_screen_pregame_lobby::create_list_widget(const s_list_widget_block* definition)
{
	return INVOKE_CLASS_MEMBER(0x00B21370, c_gui_screen_pregame_lobby, create_list_widget, definition);
}

void c_gui_screen_pregame_lobby::dispose()
{
	//return INVOKE_CLASS_MEMBER(0x00B21400, c_gui_screen_pregame_lobby, dispose);

	c_gui_screen_widget::dispose();
}

e_campaign_id __cdecl c_gui_screen_pregame_lobby::get_current_campaign_id()
{
	//return INVOKE(0x00B21410, c_gui_screen_pregame_lobby::get_current_campaign_id);

	e_gui_game_mode game_mode = user_interface_squad_get_ui_game_mode();
	if (game_mode != _ui_game_mode_campaign)
	{
		return _campaign_id_none;
	}

	e_campaign_id campaign_id = {};
	e_map_id map_id = {};
	if (!user_interface_session_get_map(&campaign_id, &map_id))
	{
		return _campaign_id_none;
	}

	return campaign_id;
}

e_map_id __cdecl c_gui_screen_pregame_lobby::get_current_map_id()
{
	//return INVOKE(0x00B21450, c_gui_screen_pregame_lobby::get_current_map_id);

	e_gui_game_mode game_mode = user_interface_squad_get_ui_game_mode();
	if (game_mode != _ui_game_mode_campaign &&
		game_mode != _ui_game_mode_multiplayer &&
		game_mode != _ui_game_mode_map_editor &&
		game_mode != _ui_game_mode_theater)
	{
		return _map_id_none;
	}

	e_campaign_id campaign_id = {};
	e_map_id map_id = {};
	if (!user_interface_session_get_map(&campaign_id, &map_id))
	{
		return _map_id_none;
	}

	return map_id;
}

const c_game_variant* c_gui_screen_pregame_lobby::get_current_variant()
{
	//return INVOKE(0x00B214A0, c_gui_screen_pregame_lobby::get_current_variant);

	e_gui_game_mode game_mode = user_interface_squad_get_ui_game_mode();
	if (game_mode != _ui_game_mode_multiplayer &&
		game_mode != _ui_game_mode_map_editor &&
		game_mode != _ui_game_mode_theater)
	{
		return NULL;
	}

	return user_interface_game_settings_get_game_variant();
}

//.text:00B214D0 ; 

int32 c_gui_screen_pregame_lobby::get_options_button_name(bool is_leader)
{
	int32 result = _string_id_invalid;
	switch (m_name)
	{
	case STRING_ID(gui, pregame_lobby_campaign):
	{
		result = is_leader ? STRING_ID(gui, options_leader_campaign) : STRING_ID(gui, options_member_campaign);
	}
	break;
	case STRING_ID(gui, pregame_lobby_matchmaking):
	{
		result = is_leader ? STRING_ID(gui, options_leader_matchmaking) : STRING_ID(gui, options_member_matchmaking);
	}
	break;
	case STRING_ID(gui, pregame_lobby_multiplayer):
	{
		result = is_leader ? STRING_ID(gui, options_leader_multiplayer) : STRING_ID(gui, options_member_multiplayer);
	}
	break;
	case STRING_ID(gui, pregame_lobby_mapeditor):
	{
		result = is_leader ? STRING_ID(gui, options_leader_mapeditor) : STRING_ID(gui, options_member_mapeditor);
	}
	break;
	case STRING_ID(gui, pregame_lobby_theater):
	{
		result = is_leader ? STRING_ID(gui, options_leader_theater) : STRING_ID(gui, options_member_theater);
	}
	break;
	}

	return result;
}

int32 __cdecl c_gui_screen_pregame_lobby::get_start_status_text(bool is_leader)
{
	return INVOKE(0x00B214E0, c_gui_screen_pregame_lobby::get_start_status_text, is_leader);
}

bool c_gui_screen_pregame_lobby::handle_back_out(e_controller_index controller_index)
{
	return INVOKE_CLASS_MEMBER(0x00B21930, c_gui_screen_pregame_lobby, handle_back_out, controller_index);
}

bool c_gui_screen_pregame_lobby::handle_controller_input_message(const c_controller_input_message* message)
{
	return INVOKE_CLASS_MEMBER(0x00B21A20, c_gui_screen_pregame_lobby, handle_controller_input_message, message);

	//bool in_or_after_countdown = user_interface_squad_in_or_after_countdown();
	//
	//if (message->get_event_type() == _event_type_tab_left || message->get_event_type() == _event_type_tab_right)
	//{
	//	if (in_or_after_countdown)
	//	{
	//		return c_gui_screen_widget::handle_controller_input_message(message);
	//	}
	//
	//	c_gui_list_widget* lobby_list = c_gui_widget::get_child_list_widget(STRING_ID(gui, lobby_list));
	//	c_gui_list_widget* roster = c_gui_widget::get_child_list_widget(STRING_ID(gui, roster));
	//	c_gui_widget* focused_widget = c_gui_screen_widget::get_focused_widget();
	//	if (!lobby_list || !roster || !focused_widget || focused_widget->m_type != _gui_list_item)
	//	{
	//		return c_gui_screen_widget::handle_controller_input_message(message);
	//	}
	//
	//	bool focused_parent_list_is_lobby = focused_widget->get_parent_list() == lobby_list;
	//	if (message->get_event_type() == _event_type_tab_right && focused_parent_list_is_lobby)
	//	{
	//		c_gui_screen_widget::transfer_focus_to_list(roster, roster->get_focused_element_handle(), true, true);
	//		return true;
	//	}
	//	if (message->get_event_type() == _event_type_tab_left && !focused_parent_list_is_lobby && user_interface_squad_local_peer_is_leader())
	//	{
	//		c_gui_screen_widget::transfer_focus_to_list(roster, lobby_list->get_focused_element_handle(), true, true);
	//		return true;
	//	}
	//}
	//
	//if (message->get_event_type() != _event_type_button_press)
	//{
	//	return c_gui_screen_widget::handle_controller_input_message(message);
	//}
	//
	//switch (message->get_event_type())
	//{
	//break;
	//case _controller_component_button_a:
	//{
	//	if (in_or_after_countdown)
	//	{
	//		user_interface_squad_start_countdown_timer(message->get_controller(), 6, 4);
	//		return false;
	//	}
	//
	//	if (!c_gui_screen_pregame_lobby::team_switching_allowed())
	//	{
	//		break;
	//	}
	//
	//	c_gui_roster_list_widget* roster_list_widget = (c_gui_roster_list_widget*)c_gui_widget::get_child_list_widget(STRING_ID(gui, roster));
	//	if (!roster_list_widget)
	//	{
	//		break;
	//	}
	//
	//	int32 player_at_local_controller = user_interface_session_get_player_at_local_controller(message->get_controller());
	//	if (player_at_local_controller == NONE || !roster_list_widget->get_current_team_change_is_active(player_at_local_controller))
	//	{
	//		break;
	//	}
	//
	//	int32 team_change_lying_time = (int32)user_interface_milliseconds() + c_gui_screen_widget::get_constants_datasource_integer(STRING_ID(gui, team_change_lying_time));
	//	c_gui_screen_pregame_lobby::commit_team_change(player_at_local_controller, roster_list_widget, team_change_lying_time);
	//	return true;
	//}
	//break;
	//case _controller_component_button_b:
	//{
	//	if (!in_or_after_countdown)
	//	{
	//		if (!c_gui_screen_pregame_lobby::team_switching_allowed())
	//		{
	//			c_gui_screen_pregame_lobby::handle_back_out(message->get_controller());
	//			return true;
	//		}
	//
	//		c_gui_roster_list_widget* roster_list_widget = (c_gui_roster_list_widget*)c_gui_widget::get_child_list_widget(STRING_ID(gui, roster));
	//		if (!roster_list_widget)
	//		{
	//			c_gui_screen_pregame_lobby::handle_back_out(message->get_controller());
	//			return true;
	//		}
	//
	//		int32 player_at_local_controller = user_interface_session_get_player_at_local_controller(message->get_controller());
	//		if (player_at_local_controller == NONE || !roster_list_widget->get_current_team_change_is_active(player_at_local_controller))
	//		{
	//			c_gui_screen_pregame_lobby::handle_back_out(message->get_controller());
	//			return true;
	//		}
	//
	//		int32 team_change_lying_time = (int32)user_interface_milliseconds() + c_gui_screen_widget::get_constants_datasource_integer(STRING_ID(gui, team_change_lying_time));
	//		roster_list_widget->end_team_change(player_at_local_controller, 0);
	//		return true;
	//	}
	//
	//	if (!user_interface_squad_local_peer_is_leader())
	//	{
	//		c_gui_screen_pregame_lobby::handle_back_out(message->get_controller());
	//		return true;
	//	}
	//
	//	user_interface_squad_stop_countdown_timer(message->get_controller(), -1, false);
	//	return true;
	//}
	//break;
	//case _controller_component_button_x:
	//{
	//	if (in_or_after_countdown)
	//	{
	//		user_interface_squad_stop_countdown_timer(message->get_controller(), -1, false);
	//	}
	//	return true;
	//}
	//break;
	//case _controller_component_button_y:
	//{
	//	if (online_is_connected_to_live())
	//	{
	//		online_guide_show_friends_ui(message->get_controller());
	//	}
	//	return true;
	//}
	//break;
	//case _controller_component_button_left_shoulder:
	//case _controller_component_button_right_shoulder:
	//{
	//	if (!in_or_after_countdown || !team_switching_allowed())
	//	{
	//		break;
	//	}
	//
	//	c_gui_roster_list_widget* roster_list_widget = (c_gui_roster_list_widget*)c_gui_widget::get_child_list_widget(STRING_ID(gui, roster));
	//	if (!roster_list_widget)
	//	{
	//		break;
	//	}
	//
	//	const c_game_variant* current_variant = c_gui_screen_pregame_lobby::get_current_variant();
	//	if (!current_variant)
	//	{
	//		break;
	//	}
	//
	//	e_map_id current_map_id = c_gui_screen_pregame_lobby::get_current_map_id();
	//	if (!current_map_id)
	//	{
	//		break;
	//	}
	//
	//	int32 player_at_local_controller = user_interface_session_get_player_at_local_controller(message->get_controller());
	//	if (player_at_local_controller == NONE)
	//	{
	//		break;
	//	}
	//
	//	const s_player_configuration* player_data = user_interface_session_get_player_data(player_at_local_controller);
	//	int32 maximum_team = game_engine_variant_get_maximum_team_count(current_variant, current_map_id);
	//}
	//break;
	//}
	//
	//return c_gui_screen_widget::handle_controller_input_message(message);
}

bool c_gui_screen_pregame_lobby::handle_dialog_result(const c_dialog_result_message* message)
{
	return INVOKE_CLASS_MEMBER(0x00B21E20, c_gui_screen_pregame_lobby, handle_dialog_result, message);
}

bool c_gui_screen_pregame_lobby::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00B21EA0, c_gui_screen_pregame_lobby, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	if (list_name == STRING_ID(gui, lobby_list))
	{
		int32 target_name = _string_id_invalid;
		int32 element_handle = list_item_widget->get_element_handle();
		if (datasource->get_string_id_value(element_handle, STRING_ID(gui, target), &target_name))
		{
			if (target_name == STRING_ID(gui, network_mode))
			{
				if (c_load_pregame_selection_screen_message* screen_message = UI_MALLOC(c_load_pregame_selection_screen_message,
					message->get_controller(),
					c_gui_screen_widget::get_render_window(),
					m_name,
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
			if (target_name == STRING_ID(gui, switch_lobby))
			{
				if (c_load_screen_message* screen_message = UI_MALLOC(c_load_screen_message,
					STRING_ID(gui, pregame_switch_lobby),
					k_any_controller,
					c_gui_screen_widget::get_render_window(),
					m_name))
				{
					screen_message->set_parent_screen_index(m_screen_index);
					screen_message->set_focus_on_load_by_name(
						STRING_ID(gui, switch_lobby),
						STRING_ID(global, name),
						user_interface_networking_get_name_from_gui_game_mode(get_gui_game_mode()));
					user_interface_messaging_post(screen_message);
				}
				return true;
			}
			if (target_name == STRING_ID(gui, start_game))
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
					c_gui_screen_widget::play_sound(_ui_global_sound_effect_failure);
				}
				return true;
			}
		}
	}
	else if (list_name == STRING_ID(gui, roster))
	{
		int32 target_session_player_index = NONE;
		int32 element_handle = list_item_widget->get_element_handle();
		if (datasource->get_integer_value(element_handle, STRING_ID(gui, player_index), &target_session_player_index))
		{
			if (const s_player_identifier* player_identifier = user_interface_session_get_player_identifier(target_session_player_index))
			{
				if (select_player_in_roster(message->get_controller(), player_identifier))
				{
					return true;
				}
			}
		}
	}

	return c_gui_screen_widget::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

bool c_gui_screen_pregame_lobby::postgame_stats_enabled()
{
	//return INVOKE_CLASS_MEMBER(0x00B22130, c_gui_screen_pregame_lobby, postgame_stats_enabled);

	return user_interface_networking_get_final_game_results() != NULL;
}

void c_gui_screen_pregame_lobby::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00B22140, c_gui_screen_pregame_lobby, initialize);

	c_gui_screen_widget::initialize();

	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-title", this, parse_xml_lobby_title));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-header", this, parse_xml_lobby_header));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-network", this, parse_xml_lobby_network));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-party-leader", this, parse_xml_lobby_party_leader));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-start-button-name", this, parse_xml_lobby_start_button_name));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-current-players", this, parse_xml_lobby_current_players));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-max-players", this, parse_xml_lobby_max_players));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-countdown-remaining", this, parse_xml_lobby_countdown_remaining));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-privacy", this, parse_xml_ui_screen_party_privacy));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-delaying-player", this, parse_xml_lobby_delaying_player));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-film-max-players", this, parse_xml_lobby_film_max_players));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-film-party-leader-requirement", this, parse_xml_lobby_film_party_leader_requirement));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-coop-max-players", this, parse_xml_lobby_coop_max_players));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-percent-loaded", this, parse_xml_lobby_percent_loaded));
}

void c_gui_screen_pregame_lobby::initialize_datasource()
{
	//INVOKE_CLASS_MEMBER(0x00B224D0, c_gui_screen_pregame_lobby, initialize_datasource);

	c_gui_screen_widget::initialize_datasource();

	c_gui_active_roster_data* active_roster = UI_MALLOC(c_gui_active_roster_data, c_gui_widget::get_driving_controller());
	if (active_roster && active_roster->initialize(STRING_ID(gui, roster)))
	{
		c_gui_screen_widget::add_datasource(active_roster);
	}
	else
	{
		ui_track_delete<c_gui_active_roster_data>(active_roster);
	}
}

//.text:00B22580 ; private: bool c_gui_screen_pregame_lobby::is_team_game()

void c_gui_screen_pregame_lobby::load_game_variant_editing_screen(e_controller_index controller_index)
{
	INVOKE_CLASS_MEMBER(0x00B225B0, c_gui_screen_pregame_lobby, load_game_variant_editing_screen, controller_index);

	//if (user_interface_session_is_verifying_strings())
	//{
	//	event(_event_message, "ui: can't load game editing screen because strings are still being verified");
	//	return;
	//}
	//
	//const c_game_variant* game_variant = user_interface_game_settings_get_game_variant();
	//if (!game_variant)
	//{
	//	event(_event_message, "ui: user_interface_game_settings_get_game_variant() returned NULL");
	//	return;
	//}
	//
	//e_game_engine_category interface_category = k_invalid_category;
	//switch (game_variant->get_game_engine_index())
	//{
	//case _game_engine_type_ctf:
	//{
	//	interface_category = _game_variant_ctf_top_category_options;
	//}
	//break;
	//case _game_engine_type_slayer:
	//{
	//	interface_category = _game_variant_slayer_top_category_options;
	//}
	//break;
	//case _game_engine_type_oddball:
	//{
	//	interface_category = _game_variant_oddball_top_category_options;
	//}
	//break;
	//case _game_engine_type_king:
	//{
	//	interface_category = _game_variant_king_top_category_options;
	//}
	//break;
	//case _game_engine_type_sandbox:
	//{
	//	interface_category = _game_variant_sandbox_top_category_options;
	//}
	//break;
	//case _game_engine_type_vip:
	//{
	//	interface_category = _game_variant_vip_top_category_options;
	//}
	//break;
	//case _game_engine_type_juggernaut:
	//{
	//	interface_category = _game_variant_juggernaut_top_category_options;
	//}
	//break;
	//case _game_engine_type_territories:
	//{
	//	interface_category = _game_variant_territories_top_category_options;
	//}
	//break;
	//case _game_engine_type_assault:
	//{
	//	interface_category = _game_variant_assault_top_category_options;
	//}
	//break;
	//case _game_engine_type_infection:
	//{
	//	interface_category = _game_variant_infection_top_category_options;
	//}
	//break;
	//{
	//	event(_event_error, "ui: unhandled game variant type for options display!");
	//	return;
	//}
	//break;
	//}
	//
	//const c_game_variant* edit_game_variant = user_interface_game_settings_get_game_variant();
	//if (!edit_game_variant)
	//{
	//	event(_event_error, "ui: no current game variant available to display options!");
	//	return;
	//}
	//
	//c_load_game_options_screen_message* game_options_screen_message = c_load_game_options_screen_message();
	//if (!game_options_screen_message)
	//{
	//	return;
	//}
	//
	//game_options_screen_message->set_transition_type(_screen_transition_type_normal);
	//user_interface_messaging_post(game_options_screen_message);
}

void c_gui_screen_pregame_lobby::post_initialize()
{
	//INVOKE_CLASS_MEMBER(0x00B22AF0, c_gui_screen_pregame_lobby, post_initialize);

	c_gui_screen_widget::post_initialize();

	if (!location_manager_get()->get_show_postgame_stats_upon_lobby_entrance())
	{
		return;
	}

	c_load_screen_message* screen_message = UI_MALLOC(c_load_screen_message, STRING_ID(gui, carnage_report), k_any_controller, k_number_of_player_windows, m_name);
	if (screen_message)
	{
		user_interface_messaging_post(screen_message);
	}

	location_manager_get()->set_show_postgame_stats_upon_lobby_entrance(false);
}

bool c_gui_screen_pregame_lobby::select_player_in_roster(e_controller_index controller_index, const s_player_identifier* target_player_identifier)
{
	return INVOKE_CLASS_MEMBER(0x00B22B80, c_gui_screen_pregame_lobby, select_player_in_roster, controller_index, target_player_identifier);
}

//.text:00B22C90 ; private: void c_gui_screen_pregame_lobby::show_vidmaster_popup_if_needed()?

void c_gui_screen_pregame_lobby::start_fade_during_countdown()
{
	//INVOKE_CLASS_MEMBER(0x00B22DB0, c_gui_screen_pregame_lobby, start_fade_during_countdown);

	window_manager_get()->set_fading(true);
}

void c_gui_screen_pregame_lobby::stop_fade_during_countdown()
{
	//INVOKE_CLASS_MEMBER(0x00B22DC0, c_gui_screen_pregame_lobby, stop_fade_during_countdown);

	window_manager_get()->set_fading(false);
}

bool c_gui_screen_pregame_lobby::team_switching_allowed()
{
	//return INVOKE_CLASS_MEMBER(0x00B22DD0, c_gui_screen_pregame_lobby, team_switching_allowed);

	return user_interface_networking_should_allow_team_switching();
}

void c_gui_screen_pregame_lobby::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00B22DE0, c_gui_screen_pregame_lobby, update, current_milliseconds);
}

//.text:00B23220 ; private: void c_gui_screen_pregame_lobby::update_precaching(uns32)

void c_gui_screen_pregame_lobby::update_status()
{
	//INVOKE_CLASS_MEMBER(0x00B23290, c_gui_screen_pregame_lobby, update_status);

	c_gui_text_widget* lobby_status_text_widget = c_gui_widget::get_child_text_widget(STRING_ID(gui, lobby_status));
	c_gui_list_item_widget* start_game_list_item_widget = c_gui_screen_widget::find_list_item(STRING_ID(gui, lobby_list), STRING_ID(global, name), STRING_ID(gui, start_game));
	bool error = user_interface_get_session_game_start_status(NULL, NULL) == _session_game_start_status_error;
	bool is_leader = user_interface_squad_local_peer_is_leader();
	int32 start_status_text = c_gui_screen_pregame_lobby::get_start_status_text(is_leader);
	c_gui_screen_widget::set_text_widget_string_id(STRING_ID(gui, lobby_status), start_status_text);

	if (lobby_status_text_widget)
	{
		lobby_status_text_widget->set_use_alternate_ambient_state(error);
	}

	if (start_game_list_item_widget)
	{
		start_game_list_item_widget->set_enabled(is_leader && !user_interface_squad_in_or_after_countdown() && !error);
	}
}

//.text:00B23340 ; private: void c_gui_screen_pregame_lobby::update_vidmaster_popup()?

void c_gui_screen_pregame_lobby::update_widget_visiblility()
{
	//INVOKE_CLASS_MEMBER(0x00B23440, c_gui_screen_pregame_lobby, update_widget_visiblility);

	bool is_leader = user_interface_squad_local_peer_is_leader();
	bool in_or_after_countdown = user_interface_squad_in_or_after_countdown();

	c_gui_bitmap_widget* advanced_options_bitmap_widget = c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, advanced_options));
	c_gui_text_widget* advanced_options_text_widget = c_gui_widget::get_child_text_widget(STRING_ID(gui, advanced_options));
	c_gui_text_widget* change_teams_text_widget = c_gui_widget::get_child_text_widget(STRING_ID(global, change_teams));
	c_gui_text_widget* postgame_stats_text_widget = c_gui_widget::get_child_text_widget(STRING_ID(gui, postgame_stats));

	bool advanced_options_visible = !in_or_after_countdown && advanced_options_enabled();

	if (advanced_options_text_widget)
	{
		advanced_options_text_widget->set_text_from_string_id(this, get_options_button_name(is_leader));
		advanced_options_text_widget->set_use_alternate_ambient_state(!advanced_options_visible);

		if (advanced_options_visible != m_advanced_options_visible)
		{
			advanced_options_text_widget->start_animation(advanced_options_visible ? _custom_animation0 : _custom_animation1, 0);
		}
	}

	if (advanced_options_bitmap_widget)
	{
		advanced_options_bitmap_widget->set_use_alternate_ambient_state(!advanced_options_visible);

		if (advanced_options_visible != m_advanced_options_visible)
		{
			advanced_options_bitmap_widget->start_animation(advanced_options_visible ? _custom_animation0 : _custom_animation1, false);
		}
	}

	m_advanced_options_visible = advanced_options_visible;

	if (change_teams_text_widget)
	{
		bool change_teams_visible = !in_or_after_countdown && team_switching_allowed();
		change_teams_text_widget->set_use_alternate_ambient_state(!change_teams_visible);

		if (change_teams_visible != m_change_teams_visible)
		{
			change_teams_text_widget->start_animation(change_teams_visible ? _custom_animation0 : _custom_animation1, false);
		}

		m_change_teams_visible = change_teams_visible;
	}

	if (postgame_stats_text_widget)
	{
		bool postgame_stats_visible = !in_or_after_countdown && user_interface_networking_get_final_game_results();
		postgame_stats_text_widget->set_use_alternate_ambient_state(!postgame_stats_visible);

		if (postgame_stats_visible != m_postgame_stats_visible)
		{
			postgame_stats_text_widget->start_animation(postgame_stats_visible ? _custom_animation0 : _custom_animation1, false);
		}

		m_postgame_stats_visible = postgame_stats_visible;
	}
}

