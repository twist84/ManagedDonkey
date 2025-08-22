#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_matchmaking.hpp"

#include "cseries/cseries_events.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/gui_custom_bitmap_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_matchmaking.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "memory/module.hpp"
#include "networking/online/online_url.hpp"

//HOOK_DECLARE_CLASS_MEMBER(0x00B01920, c_gui_screen_pregame_lobby_matchmaking, handle_list_item_chosen_);
HOOK_DECLARE_CLASS_MEMBER(0x00B01F50, c_gui_screen_pregame_lobby_matchmaking, update_nightmap_);
//HOOK_DECLARE_CLASS_MEMBER(0x00B02070, c_gui_screen_pregame_lobby_matchmaking, update_population_);

//bool __thiscall c_gui_screen_pregame_lobby_matchmaking::handle_list_item_chosen_(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
//{
//	return c_gui_screen_pregame_lobby_matchmaking::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
//}

void __thiscall c_gui_screen_pregame_lobby_matchmaking::update_nightmap_()
{
	c_gui_screen_pregame_lobby_matchmaking::update_nightmap();
}

void __thiscall c_gui_screen_pregame_lobby_matchmaking::update_population_()
{
	c_gui_screen_pregame_lobby_matchmaking::update_population();
}

//.text:00B01500 ; public: c_http_blf_simple_downloader<s_dynamic_matchmaking_hopper_statistics>::c_http_blf_simple_downloader<s_dynamic_matchmaking_hopper_statistics>()
//.text:00B01540 ; public: c_http_stored_buffer_downloader<102400>::c_http_stored_buffer_downloader<102400>()

c_gui_screen_pregame_lobby_matchmaking::c_gui_screen_pregame_lobby_matchmaking(int32 name) :
	c_gui_screen_pregame_lobby(name),
	m_hopper_statstics_downloader(),
	m_url_key(0),
	m_nightmap_downloader(),
	m_nightmap_download_completed(false),
	m_last_hopper_catalog_load_status(_network_file_load_none),
	m_statistics_current_hopper_id(0xFFFFi16),
	m_statistics_current_statistics()
{
	//DECLFUNC(0x00B01580, void, __thiscall, c_gui_screen_pregame_lobby_matchmaking*, int32)(this, name);

	DECLFUNC(0x00B01500, void, __thiscall, c_http_blf_simple_downloader<s_dynamic_matchmaking_hopper_statistics, 2465>*)(&m_hopper_statstics_downloader);
	DECLFUNC(0x00B01540, void, __thiscall, c_http_stored_buffer_downloader<102400>*)(&m_nightmap_downloader);

	zero_array(pad);
}

//.text:00B01660 ; public: virtual c_http_stored_buffer_downloader<102400>::~c_http_stored_buffer_downloader<102400>()
//.text:00B01670 ; public: virtual c_http_stored_buffer_downloader<2465>::~c_http_stored_buffer_downloader<2465>()
//.text:00B01680 ; 
//.text:00B01690 ; public: virtual c_gui_screen_pregame_lobby_matchmaking::~c_gui_screen_pregame_lobby_matchmaking()
//.text:00B01700 ; public: virtual void* c_http_stored_buffer_downloader<102400>::`vector deleting destructor'(unsigned int)
//.text:00B01730 ; public: virtual void* c_http_stored_buffer_downloader<2465>::`vector deleting destructor'(unsigned int)
//.text:00B01760 ; public: virtual void* c_gui_screen_pregame_lobby_matchmaking::`vector deleting destructor'(unsigned int)

c_gui_bitmap_widget* c_gui_screen_pregame_lobby_matchmaking::create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00B017F0, c_gui_screen_pregame_lobby_matchmaking, create_bitmap_widget, definition);

	if (definition->widget_identifier == STRING_ID(gui, nightmap))
	{
		c_gui_custom_bitmap_widget* nightmap_bitmap_widget = UI_MALLOC(c_gui_custom_bitmap_widget);
		if (!nightmap_bitmap_widget)
		{
			return NULL;
		}
		return nightmap_bitmap_widget;
	}

	return c_gui_widget::create_bitmap_widget(definition);
}

e_gui_game_mode c_gui_screen_pregame_lobby_matchmaking::get_gui_game_mode()
{
	//return INVOKE_CLASS_MEMBER(0x00B01870, c_gui_screen_pregame_lobby_matchmaking, get_gui_game_mode);

	return _ui_game_mode_matchmaking;
}

//.text:00B01880 ; public: const s_matchmaking_single_hopper_statistics* c_gui_screen_pregame_lobby_matchmaking::get_hopper_statistics()

int32 c_gui_screen_pregame_lobby_matchmaking::get_lobby_header()
{
	//return INVOKE_CLASS_MEMBER(0x00B018B0, c_gui_screen_pregame_lobby_matchmaking, get_lobby_header);

	return STRING_ID(gui, header_matchmaking);
}

int32 c_gui_screen_pregame_lobby_matchmaking::get_lobby_title()
{
	//return INVOKE_CLASS_MEMBER(0x00B018C0, c_gui_screen_pregame_lobby_matchmaking, get_lobby_title);

	return STRING_ID(gui, title_matchmaking);
}

int32 c_gui_screen_pregame_lobby_matchmaking::get_start_button_name()
{
	//return INVOKE_CLASS_MEMBER(0x00B018D0, c_gui_screen_pregame_lobby_matchmaking, get_start_button_name);

	return STRING_ID(gui, start_matchmaking);
}

bool c_gui_screen_pregame_lobby_matchmaking::handle_controller_input_message(const c_controller_input_message* message)
{
	if (message->get_event_type() == _event_type_button_press && message->get_component() == _controller_component_button_x)
	{
		if (user_interface_squad_in_or_after_countdown())
		{
			user_interface_squad_stop_countdown_timer(message->get_controller(), -1, false);
			return true;
		}

		if (c_load_screen_message* screen_message = UI_MALLOC(c_load_screen_message,
			STRING_ID(gui, pregame_advanced_options_matchmaking),
			message->get_controller(),
			c_gui_screen_widget::get_render_window(),
			m_name))
		{
			screen_message->set_parent_screen_index(m_screen_index);
			user_interface_messaging_post(screen_message);
		}
		return true;
	}

	return c_gui_screen_pregame_lobby::handle_controller_input_message(message);
}

bool c_gui_screen_pregame_lobby_matchmaking::handle_dialog_result(const c_dialog_result_message* message)
{
	return INVOKE_CLASS_MEMBER(0x00B018E0, c_gui_screen_pregame_lobby_matchmaking, handle_dialog_result, message);

	//if (message->get_dialog_name() != STRING_ID(gui_dialog, matchmaking_lobby_switch_to_live))
	//{
	//	return c_gui_screen_pregame_lobby::handle_dialog_result(message);
	//}
	//
	//if (message->get_dialog_result() == _gui_dialog_choice_first)
	//{
	//	user_interface_networking_set_session_advertisement(global_preferences_get_xbox_live_private_privacy_setting());
	//}
	//
	//return true;
}

bool c_gui_screen_pregame_lobby_matchmaking::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00B01920, c_gui_screen_pregame_lobby_matchmaking, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	if (list_name == STRING_ID(gui, lobby_list))
	{
		int32 target_name = _string_id_invalid;
		if (datasource->get_string_id_value(list_item_widget->get_element_handle(), STRING_ID(gui, target), &target_name))
		{
			switch (target_name)
			{
			case STRING_ID(gui, advanced_options):
			{
				if (c_load_screen_message* screen_message = UI_MALLOC(c_load_screen_message,
					STRING_ID(gui, pregame_advanced_options_matchmaking),
					message->get_controller(),
					c_gui_screen_widget::get_render_window(),
					m_name))
				{
					screen_message->set_parent_screen_index(m_screen_index);
					user_interface_messaging_post(screen_message);
				}
				return true;
			}
			break;
			case STRING_ID(global, hopper):
			{
				if (!is_lobby_in_live_mode())
				{
					if (c_load_dialog_screen_message* dialog_screen_message = UI_MALLOC(c_load_dialog_screen_message,
						message->get_controller(),
						c_gui_screen_widget::get_render_window(),
						m_name,
						STRING_ID(gui_dialog, matchmaking_lobby_switch_to_live),
						m_name))
					{
						user_interface_messaging_post(dialog_screen_message);
					}
				}
				else
				{
					if (user_interface_matchmaking_hopper_catalog_load_status() == _network_files_load_complete)
					{
						if (c_load_pregame_selection_screen_message* pregame_selection_screen_message = UI_MALLOC(c_load_pregame_selection_screen_message,
							message->get_controller(),
							c_gui_screen_widget::get_render_window(),
							m_name,
							_gui_selection_type_hopper))
						{
							pregame_selection_screen_message->set_parent_screen_index(m_screen_index);
							user_interface_messaging_post(pregame_selection_screen_message);
						}
					}
				}
				return true;
			}
			break;
			}
		}
	}

	return c_gui_screen_pregame_lobby::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

void c_gui_screen_pregame_lobby_matchmaking::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00B01B10, c_gui_screen_pregame_lobby_matchmaking, initialize);

	c_gui_screen_pregame_lobby::initialize();

	m_initial_focused_widget = STRING_ID(gui, lobby_list);
	m_initial_focused_widget_element_handle = 3;

	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-matchmaking-hopper", this, parse_xml_lobby_matchmaking_hopper));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-matchmaking-hopper-population", this, parse_xml_lobby_matchmaking_hopper_population));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-matchmaking-total-population", this, parse_xml_lobby_matchmaking_total_population));
}

bool c_gui_screen_pregame_lobby_matchmaking::is_lobby_in_live_mode()
{
	return INVOKE_CLASS_MEMBER(0x00B01C10, c_gui_screen_pregame_lobby_matchmaking, is_lobby_in_live_mode);
}

bool __cdecl parse_xml_lobby_matchmaking_hopper(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B01C40, parse_xml_lobby_matchmaking_hopper, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_matchmaking_hopper_population(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B01D20, parse_xml_lobby_matchmaking_hopper_population, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_matchmaking_total_population(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B01DA0, parse_xml_lobby_matchmaking_total_population, this_ptr, buffer, buffer_length);
}

void c_gui_screen_pregame_lobby_matchmaking::start_fade_during_countdown()
{
	//INVOKE_CLASS_MEMBER(0x00B01E20, c_gui_screen_pregame_lobby_matchmaking, start_fade_during_countdown);
}

void c_gui_screen_pregame_lobby_matchmaking::stop_fade_during_countdown()
{
	//INVOKE_CLASS_MEMBER(0x00B01E30, c_gui_screen_pregame_lobby_matchmaking, stop_fade_during_countdown);
}

void c_gui_screen_pregame_lobby_matchmaking::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00B01E40, c_gui_screen_pregame_lobby_matchmaking, update, current_milliseconds);

	// $TODO: finish implementation

	//c_gui_screen_pregame_lobby::update(current_milliseconds);
	//
	//if (!user_interface_squad_exists() || user_interface_get_session_game_mode() != _session_game_mode_pregame)
	//{
	//	return;
	//}
	//
	//c_gui_bitmap_widget* ranked_match_bitmap_widget = (c_gui_bitmap_widget*)c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, ranked_match));
	//c_gui_bitmap_widget* hopper_bitmap_widget = (c_gui_bitmap_widget*)c_gui_widget::get_child_bitmap_widget(STRING_ID(global, hopper));
	//uns16 hopper = user_interface_matchmaking_get_hopper();
}

void c_gui_screen_pregame_lobby_matchmaking::update_nightmap()
{
	//INVOKE_CLASS_MEMBER(0x00B01F50, c_gui_screen_pregame_lobby_matchmaking, update_nightmap);

	const char* downloaded_data = NULL;
	int32 downloaded_data_length = 0;

	if (m_url_key != g_online_url_hopper_directory_key)
	{
		m_url_key = g_online_url_hopper_directory_key;
		c_url_string url{};
		online_url_make_matchmaking_nightmap(&url);
		m_nightmap_downloader.set_url(&url);
	}

	if (m_nightmap_downloader.get_data(&downloaded_data, &downloaded_data_length) != _http_download_status_succeeded)
	{
		return;
	}

	if (!downloaded_data)
	{
		return;
	}

	if (downloaded_data_length <= 0 || m_nightmap_download_completed)
	{
		return;
	}

	c_gui_custom_bitmap_widget* nightmap_bitmap_widget = (c_gui_custom_bitmap_widget*)c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, nightmap));
	m_nightmap_download_completed = true;

	if (!nightmap_bitmap_widget)
	{
		return;
	}

	s_runtime_core_widget_definition* definition = nightmap_bitmap_widget->get_core_definition();
	if (TEST_BIT(definition->flags, 3))
	{
		event(_event_error, "ui: dynamic bitmaps must not have the scale to fit bit checked");
		return;
	}

	int32 width = int32(definition->authored_bounds.x1 - definition->authored_bounds.x0);
	int32 height = int32(definition->authored_bounds.y1 - definition->authored_bounds.y0);
	if (width <= 0 || height <= 0)
	{
		event(_event_error, "ui: dynamic bitmaps must have width > 0, height > 0");
		return;
	}

	nightmap_bitmap_widget->load_from_buffer(true, downloaded_data, downloaded_data_length, _custom_bitmap_desired_aspect_ratio_stretch_to_fit);
	c_gui_widget::set_child_visible(_gui_bitmap, STRING_ID(gui, nightmap_background), false);
	c_gui_widget::set_child_visible(_gui_bitmap, STRING_ID(gui, nightmap_overlay), false);
}

void c_gui_screen_pregame_lobby_matchmaking::update_population()
{
	INVOKE_CLASS_MEMBER(0x00B02070, c_gui_screen_pregame_lobby_matchmaking, update_population);

	//bool hopper_information_available = false;
	//if (user_interface_matchmaking_hopper_catalog_load_status() == _network_files_load_complete)
	//{
	//	s_hopper_information hopper_information{};
	//	if (user_interface_matchmaking_get_hopper_information(user_interface_matchmaking_get_hopper(), &hopper_information))
	//	{
	//		hopper_information_available = true;
	//	}
	//}
	//
	//user_interface_matchmaking_update_hopper_statistics_for_screen(&m_hopper_statstics_downloader, &m_statistics_current_hopper_id, &m_statistics_current_statistics);
	//
	//s_matchmaking_single_hopper_statistics* current_statistics = NULL;
	//bool total_population_visible = false;
	//
	//if (m_statistics_current_hopper_id != 0xFFFFi16 && m_statistics_current_hopper_id == user_interface_matchmaking_get_hopper())
	//{
	//	current_statistics = &m_statistics_current_statistics;
	//	if (current_statistics && current_statistics->total_online_population > 0)
	//	{
	//		total_population_visible = true;
	//	}
	//}
	//
	//bool hopper_population_available = current_statistics && current_statistics->current_hopper_population > 0;
	//c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, hopper_name), hopper_information_available && !hopper_population_available);
	//c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, hopper_population), hopper_information_available && hopper_population_available);
	//c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, total_population), total_population_visible);
}

