#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_campaign.hpp"

#include "interface/c_gui_list_item_widget.hpp"
#include "interface/gui_selected_items.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "memory/module.hpp"
#include "shell/shell.hpp"
#include "interface/user_interface.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AFFA90, c_gui_screen_pregame_lobby_campaign, handle_controller_input_message_);
HOOK_DECLARE_CLASS_MEMBER(0x00AFFAE0, c_gui_screen_pregame_lobby_campaign, handle_list_item_chosen_);
//HOOK_DECLARE_CLASS_MEMBER(0x00AFFDA0, c_gui_screen_pregame_lobby_campaign, initialize_);
//HOOK_DECLARE_CLASS_MEMBER(0x00AFFF60, c_gui_screen_pregame_lobby_campaign, load_progress_ui_);

//.text:00AFF9D0 ; public: virtual enum e_gui_game_mode c_gui_screen_pregame_lobby_campaign::get_gui_game_mode()
//.text:00AFF9E0 ; public: virtual enum e_gui_location c_gui_screen_pregame_lobby::get_gui_location() const
//.text:00AFF9F0 ; public: virtual long c_gui_screen_pregame_lobby_campaign::get_lobby_header()
//.text:00AFFA00 ; public: virtual long c_gui_screen_pregame_lobby_campaign::get_lobby_title()
//.text:00AFFA10 ; protected: virtual enum e_render_data_size c_gui_screen_pregame_lobby::get_render_data_size()
//.text:00AFFA20 ; public: virtual long c_gui_screen_pregame_lobby_campaign::get_start_button_name()
//.text:00AFFA80 ; 

bool __thiscall c_gui_screen_pregame_lobby_campaign::handle_controller_input_message_(c_controller_input_message const* message)
{
	if (message->get_event_type() == _event_type_button_press && message->get_component() == _controller_component_button_x)
	{
		if (user_interface_squad_in_or_after_countdown())
		{
			user_interface_squad_stop_countdown_timer(message->get_controller(), -1, false);
			return true;
		}
		if (user_interface_squad_local_peer_is_leader() /*&& !get_is_blue_disk()*/)
		{
			// $TODO: implement all of `c_gui_screen_campaign_settings`
			if (c_load_screen_message* screen_message = new c_load_screen_message(
				STRING_ID(gui, campaign_settings),
				message->get_controller(),
				get_render_window(),
				m_name.get_value()))
			{
				screen_message->set_parent_screen_index(m_screen_index);
				user_interface_messaging_post(screen_message);
			}
			return true;
		}
	}

	return INVOKE_CLASS_MEMBER(0x00B21A20, c_gui_screen_pregame_lobby, handle_controller_input_message, message);
}

bool __thiscall c_gui_screen_pregame_lobby_campaign::handle_list_item_chosen_(c_controller_input_message const* message, long list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	if (list_name == STRING_ID(gui, lobby_list))
	{
		long target_name = _string_id_invalid;
		long element_handle = list_item_widget->get_element_handle();
		if (datasource->get_string_id_value(element_handle, STRING_ID(gui, target), &target_name))
		{
			if (target_name == STRING_ID(global, level))
			{
				e_campaign_id campaign_id = _campaign_id_default;
				e_map_id map_id = _map_id_first;
				short campaign_insertion_point = 0;
				user_interface_session_get_map(&campaign_id, &map_id);
				if (c_load_campaign_select_level_screen_message* screen_message = new c_load_campaign_select_level_screen_message(
					STRING_ID(gui, campaign_select_level),
					message->get_controller(),
					get_render_window(),
					m_name.get_value(),
					_campaign_level_setup_mode_lobby,
					campaign_id,
					map_id,
					campaign_insertion_point))
				{
					screen_message->set_parent_screen_index(m_screen_index);
					user_interface_messaging_post(screen_message);
				}
				return true;
			}
			if (target_name == STRING_ID(global, difficulty))
			{
				if (c_load_campaign_select_difficulty_screen_message* screen_message = new c_load_campaign_select_difficulty_screen_message(
					STRING_ID(gui, campaign_select_difficulty),
					message->get_controller(),
					get_render_window(),
					m_name.get_value(),
					_campaign_difficulty_setup_mode_lobby,
					_campaign_id_default,
					_map_id_none,
					user_interface_game_settings_get_campaign_difficulty()))
				{
					screen_message->set_parent_screen_index(m_screen_index);
					user_interface_messaging_post(screen_message);
				}
				return true;
			}
		}
		else if (datasource->get_string_id_value(element_handle, STRING_ID(global, name), &target_name)) // ODST
		{
			if (target_name == STRING_ID(gui, select_scoring))
			{
				if (c_load_screen_message* screen_message = new c_load_screen_message(
					STRING_ID(gui, select_scoring),
					message->get_controller(),
					get_render_window(),
					m_name.get_value()))
				{
					screen_message->set_parent_screen_index(m_screen_index);
					user_interface_messaging_post(screen_message);
				}
				return true;
			}
			if (target_name == STRING_ID(gui, select_skulls))
			{
				if (c_load_screen_message* screen_message = new c_load_screen_message(
					STRING_ID(gui, campaign_select_skulls),
					message->get_controller(),
					get_render_window(),
					m_name.get_value()))
				{
					screen_message->set_parent_screen_index(m_screen_index);
					user_interface_messaging_post(screen_message);
				}
				return true;
			}
		}
	}

	return c_gui_screen_pregame_lobby::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

//.text:00AFFD80 ; 

//void __thiscall c_gui_screen_pregame_lobby_campaign::initialize_()
//{
//	INVOKE_CLASS_MEMBER(0x00B22140, c_gui_screen_pregame_lobby, initialize);
//
//	m_initial_focused_widget = STRING_ID(gui, lobby_list);
//
//	// value of 4 in halo 3 and 6 in odst/halo online
//	m_initial_focused_widget_element_handle = 6; // should we use 4 or 6?
//
//	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-campaign-level", this, parse_xml_lobby_campaign_level));
//	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-campaign-difficulty", this, parse_xml_lobby_campaign_difficulty));
//	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-campaign-insertion", this, parse_xml_lobby_campaign_insertion));
//}

//void __thiscall c_gui_screen_pregame_lobby_campaign::load_progress_ui_(e_controller_index controller_index)
//{
//	ASSERT(m_progress_dialog_start_time_milliseconds == 0);
//	// $TODO: Add `c_load_in_progress_screen_message`
//	if (c_load_in_progress_screen_message* in_progress_screen_message = new c_load_in_progress_screen_message(
//		controller_index,
//		get_render_window(),
//		STRING_ID(gui, pregame_selection_enumeration_in_progress_title),
//		STRING_ID(gui, pregame_selection_enumeration_in_progress_message)))
//	{
//		in_progress_screen_message->set_parent_screen_index(m_screen_index);
//		user_interface_messaging_post(in_progress_screen_message);
//		m_progress_dialog_start_time_milliseconds = user_interface_milliseconds();
//	}
//	user_interface_set_reload_from_persistent_storage(k_no_controller);
//}

//.text:00B00000 ; private: void c_gui_screen_pregame_lobby_campaign::restore_game_setup_from_controller_saved_game(e_controller_index)
//.text:00B00230 ; public: virtual void c_gui_screen_pregame_lobby_campaign::update(uint32)
//.text:00B00510 ; 

