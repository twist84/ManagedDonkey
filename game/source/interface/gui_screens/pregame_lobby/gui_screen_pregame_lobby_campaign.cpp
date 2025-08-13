#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_campaign.hpp"

#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/gui_pregame_setup_manager.hpp"
#include "interface/gui_selected_items.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "main/levels.hpp"
#include "memory/module.hpp"
#include "saved_games/content_catalogue.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00AFFA90, c_gui_screen_pregame_lobby_campaign, handle_controller_input_message_);
HOOK_DECLARE_CLASS_MEMBER(0x00AFFAE0, c_gui_screen_pregame_lobby_campaign, handle_list_item_chosen_);

bool __thiscall c_gui_screen_pregame_lobby_campaign::handle_controller_input_message_(const c_controller_input_message* message)
{
	return c_gui_screen_pregame_lobby_campaign::handle_controller_input_message(message);
}

bool __thiscall c_gui_screen_pregame_lobby_campaign::handle_list_item_chosen_(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return c_gui_screen_pregame_lobby_campaign::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

bool __cdecl parse_xml_lobby_campaign_difficulty(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00ADD8C0, parse_xml_lobby_campaign_difficulty, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_campaign_insertion(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	//return INVOKE(0x00ADDA00, parse_xml_lobby_campaign_insertion, this_ptr, buffer, buffer_length);

	e_campaign_id campaign_id = _campaign_id_none;
	e_map_id map_id = _map_id_none;
	if (!user_interface_session_get_map(&campaign_id, &map_id))
	{
		return false;
	}

	s_level_insertion_datum insertion{};
	int16 campaign_insertion_point = user_interface_game_settings_get_campaign_insertion_point();
	if (!levels_try_and_get_campaign_insertion(map_id, &insertion) || !VALID_INDEX(campaign_insertion_point, insertion.insertion_point_count))
	{
		return false;
	}

	ustrnzcpy(buffer, insertion.insertion_point_names[campaign_insertion_point], buffer_length);
	return true;
}

bool __cdecl parse_xml_lobby_campaign_level(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	//return INVOKE(0x00ADDA00, parse_xml_lobby_campaign_level, this_ptr, buffer, buffer_length);

	return c_gui_pregame_setup_manager::get()->get_selected_item_title(buffer, buffer_length, _gui_selection_type_level);
}

//.text:00AFE260 ; public: c_gui_screen_campaign_select_scoring::c_gui_screen_campaign_select_scoring(int32)
//.text:00AFE290 ; 
//.text:00AFE2A0 ; 
//.text:00AFE2B0 ; public: virtual void* c_gui_screen_campaign_select_scoring::`scalar deleting destructor'(unsigned int)
//.text:00AFE2E0 ; public: virtual bool c_gui_screen_campaign_select_scoring::handle_dialog_result(const c_dialog_result_message*)
//.text:00AFE330 ; public: virtual bool c_gui_screen_campaign_select_scoring::handle_list_item_chosen(const c_controller_input_message*, int32, c_gui_list_item_widget*, c_gui_data*)
//.text:00AFE400 ; public: virtual void c_gui_screen_campaign_select_scoring::initialize()

c_gui_screen_pregame_lobby_campaign::c_gui_screen_pregame_lobby_campaign(int32 name) :
	c_gui_screen_pregame_lobby(name),
	m_last_player_count(NONE),
	m_session_class(_network_session_class_offline),
	m_last_player_identifiers(),
	m_use_saved_game_from_controller(k_no_controller),
	m_progress_dialog_start_time_milliseconds(0),
	m_catalogue_enumeration_active(false)
{
	//DECLFUNC(0x00AFF7A0, void, __thiscall, c_gui_screen_pregame_lobby_campaign*, int32)(this, name);
}

//.text:00AFF800 ; public: c_load_campaign_select_level_screen_message::c_load_campaign_select_level_screen_message(e_controller_index, e_window_index, int32, e_gui_campaign_level_setup_mode, e_campaign_id, e_map_id)
//.text:00AFF840 ; 
//.text:00AFF890 ; 
//.text:00AFF8E0 ; 
//.text:00AFF8F0 ; 

//.text:00AFF900 ; public: virtual void* c_gui_screen_pregame_lobby_campaign::`scalar deleting destructor'(unsigned int)
c_gui_screen_pregame_lobby_campaign::~c_gui_screen_pregame_lobby_campaign()
{
}

e_gui_game_mode c_gui_screen_pregame_lobby_campaign::get_gui_game_mode()
{
	//return INVOKE_CLASS_MEMBER(0x00AFF9D0, c_gui_screen_pregame_lobby_campaign, get_gui_game_mode);

	return _ui_game_mode_campaign;
}

e_gui_location c_gui_screen_pregame_lobby::get_gui_location() const
{
	//return INVOKE_CLASS_MEMBER(0x00AFF9E0, c_gui_screen_pregame_lobby, get_gui_location);

	return _gui_location_pregame_lobby;
}

int32 c_gui_screen_pregame_lobby_campaign::get_lobby_header()
{
	//return INVOKE_CLASS_MEMBER(0x00AFF9F0, c_gui_screen_pregame_lobby_campaign, get_lobby_header);

	return STRING_ID(gui, header_campaign);
}

int32 c_gui_screen_pregame_lobby_campaign::get_lobby_title()
{
	//return INVOKE_CLASS_MEMBER(0x00AFFA00, c_gui_screen_pregame_lobby_campaign, get_lobby_title);

	return STRING_ID(gui, title_campaign);
}


e_render_data_size c_gui_screen_pregame_lobby::get_render_data_size()
{
	//return INVOKE_CLASS_MEMBER(0x00AFFA10, c_gui_screen_pregame_lobby, get_render_data_size);

	return k_render_data_size_large;
}

int32 c_gui_screen_pregame_lobby_campaign::get_start_button_name()
{
	return INVOKE_CLASS_MEMBER(0x00AFFA20, c_gui_screen_pregame_lobby_campaign, get_start_button_name);
}

//.text:00AFFA80 ; public: int32 c_static_array<struct s_player_identifier, 16>::get_total_element_size() const

bool c_gui_screen_pregame_lobby_campaign::handle_controller_input_message(const c_controller_input_message* message)
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
				c_gui_screen_widget::get_render_window(),
				m_name))
			{
				screen_message->set_parent_screen_index(m_screen_index);
				user_interface_messaging_post(screen_message);
			}
			return true;
		}
	}

	return c_gui_screen_pregame_lobby::handle_controller_input_message(message);
}

bool c_gui_screen_pregame_lobby_campaign::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	if (list_name == STRING_ID(gui, lobby_list))
	{
		int32 target_name = _string_id_invalid;
		int32 element_handle = list_item_widget->get_element_handle();
		if (datasource->get_string_id_value(element_handle, STRING_ID(gui, target), &target_name))
		{
			if (target_name == STRING_ID(global, level))
			{
				e_campaign_id campaign_id = _campaign_id_default;
				e_map_id map_id = _map_id_first;
				int16 campaign_insertion_point = 0;
				user_interface_session_get_map(&campaign_id, &map_id);
				if (c_load_campaign_select_level_screen_message* screen_message = new c_load_campaign_select_level_screen_message(
					STRING_ID(gui, campaign_select_level),
					message->get_controller(),
					c_gui_screen_widget::get_render_window(),
					m_name,
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
					c_gui_screen_widget::get_render_window(),
					m_name,
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
	}

	return c_gui_screen_pregame_lobby::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

bool c_gui_screen_pregame_lobby_campaign::postgame_stats_enabled()
{
	return INVOKE_CLASS_MEMBER(0x00AFFD80, c_gui_screen_pregame_lobby_campaign, postgame_stats_enabled);
}

void c_gui_screen_pregame_lobby_campaign::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00AFFDA0, c_gui_screen_pregame_lobby_campaign, initialize);

	c_gui_screen_pregame_lobby::initialize();
	
	m_initial_focused_widget = STRING_ID(gui, lobby_list);
	m_initial_focused_widget_element_handle = 4;
	
	c_gui_screen_widget::add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-campaign-level", this, parse_xml_lobby_campaign_level));
	c_gui_screen_widget::add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-campaign-difficulty", this, parse_xml_lobby_campaign_difficulty));
	c_gui_screen_widget::add_game_tag_parser(new c_magic_string_game_tag_parser(L"<lobby-campaign-insertion", this, parse_xml_lobby_campaign_insertion));
}

void c_gui_screen_pregame_lobby_campaign::load_progress_ui(e_controller_index controller_index)
{
	INVOKE_CLASS_MEMBER(0x00AFFF60, c_gui_screen_pregame_lobby_campaign, load_progress_ui, controller_index);

	//ASSERT(m_progress_dialog_start_time_milliseconds == 0);
	//// $TODO: Add `c_load_in_progress_screen_message`
	//if (c_load_in_progress_screen_message* in_progress_screen_message = new c_load_in_progress_screen_message(
	//	controller_index,
	//	c_gui_screen_widget::get_render_window(),
	//	STRING_ID(gui, pregame_selection_enumeration_in_progress_title),
	//	STRING_ID(gui, pregame_selection_enumeration_in_progress_message)))
	//{
	//	in_progress_screen_message->set_parent_screen_index(m_screen_index);
	//	user_interface_messaging_post(in_progress_screen_message);
	//	m_progress_dialog_start_time_milliseconds = user_interface_milliseconds();
	//}
	//user_interface_set_reload_from_persistent_storage(k_no_controller);
}

void c_gui_screen_pregame_lobby_campaign::restore_game_setup_from_controller_saved_game(e_controller_index controller_index)
{
	INVOKE_CLASS_MEMBER(0x00B00000, c_gui_screen_pregame_lobby_campaign, restore_game_setup_from_controller_saved_game, controller_index);
}

void c_gui_screen_pregame_lobby_campaign::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00B00230, c_gui_screen_pregame_lobby_campaign, update, current_milliseconds);

	// $TODO: implement me, more than just what delta did

	//c_gui_screen_pregame_lobby::update(current_milliseconds);
	//
	//if (user_interface_squad_exists() && user_interface_get_session_game_mode() == _session_game_mode_pregame)
	//{
	//	c_gui_bitmap_widget* difficulty_image_bitmap_widget = c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, difficulty_image));
	//	if (difficulty_image_bitmap_widget)
	//	{
	//		difficulty_image_bitmap_widget->set_sprite_frame(user_interface_game_settings_get_campaign_difficulty());
	//	}
	//}
}

//.text:00B00510 ; 

