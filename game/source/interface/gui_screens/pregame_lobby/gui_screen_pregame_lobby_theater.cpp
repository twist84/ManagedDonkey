#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_theater.hpp"

#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/gui_pregame_setup_manager.hpp"
#include "interface/gui_selected_items_map.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_session_interface.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B02B70, c_gui_screen_pregame_lobby_theater, update_);

void c_gui_screen_pregame_lobby_theater::update_(uns32 current_milliseconds)
{
	c_gui_screen_pregame_lobby_theater::update(current_milliseconds);
}

c_gui_screen_pregame_lobby_theater::c_gui_screen_pregame_lobby_theater(int32 name) :
	c_gui_screen_pregame_lobby(name),
	m_recent_film_set(false)
{
	//DECLFUNC(0x00B02570, void, __thiscall, c_gui_screen_pregame_lobby_theater*, int32)(this, name);
}

//.text:00B025A0 ; public: virtual c_gui_screen_pregame_lobby_theater::~c_gui_screen_pregame_lobby_theater()
//.text:00B025F0 ; public: virtual void* c_gui_screen_pregame_lobby_theater::`vector deleting destructor'(unsigned int)

e_gui_game_mode c_gui_screen_pregame_lobby_theater::get_gui_game_mode()
{
	//return INVOKE_CLASS_MEMBER(0x00B02660, c_gui_screen_pregame_lobby_theater, get_gui_game_mode);

	return _ui_game_mode_theater;
}

int32 c_gui_screen_pregame_lobby_theater::get_lobby_header()
{
	//return INVOKE_CLASS_MEMBER(0x00B02670, c_gui_screen_pregame_lobby_theater, get_lobby_header);

	return STRING_ID(gui, header_theater);
}

int32 c_gui_screen_pregame_lobby_theater::get_lobby_title()
{
	//return INVOKE_CLASS_MEMBER(0x00B02680, c_gui_screen_pregame_lobby_theater, get_lobby_title);

	return STRING_ID(gui, title_theater);
}

int32 c_gui_screen_pregame_lobby_theater::get_start_button_name()
{
	//return INVOKE_CLASS_MEMBER(0x00B02690, c_gui_screen_pregame_lobby_theater, get_start_button_name);

	return STRING_ID(gui, start_theater);
}

bool c_gui_screen_pregame_lobby_theater::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00B026A0, c_gui_screen_pregame_lobby_theater, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	if (list_name == STRING_ID(gui, lobby_list))
	{
		int32 target_name = _string_id_invalid;
		if (datasource->get_string_id_value(list_item_widget->get_element_handle(), STRING_ID(gui, target), &target_name))
		{
			if (target_name == STRING_ID(gui, film))
			{
				c_load_pregame_selection_screen_message* screen_message = UI_MALLOC(c_load_pregame_selection_screen_message,
					message->get_controller(),
					c_gui_screen_widget::get_render_window(),
					m_name,
					_gui_selection_type_film);
				if (screen_message)
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

void c_gui_screen_pregame_lobby_theater::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00B02790, c_gui_screen_pregame_lobby_theater, initialize);

	c_gui_screen_pregame_lobby::initialize();

	m_initial_focused_widget = STRING_ID(gui, lobby_list);
	m_initial_focused_widget_element_handle = 3;

	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-theater-film", this, parse_xml_lobby_theater_film));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-theater-game", this, parse_xml_lobby_theater_game));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-theater-map", this, parse_xml_lobby_theater_map));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-theater-film-length", this, parse_xml_lobby_theater_film_length));
}

void c_gui_screen_pregame_lobby_theater::initialize_datasource()
{
	INVOKE_CLASS_MEMBER(0x00B02910, c_gui_screen_pregame_lobby_theater, initialize_datasource);

	//c_gui_screen_pregame_lobby::initialize_datasource();
	//
	//c_gui_saved_film_subitem_datasource* recent_films = UI_MALLOC(c_gui_saved_film_subitem_datasource,
	//	c_gui_widget::get_arbitrary_responding_controller(),
	//	_saved_film_category_recent_films);
	//if (!recent_films)
	//{
	//	return;
	//}
	//
	//if (!recent_films->initialize(STRING_ID(gui, recent_films)))
	//{
	//	ui_track_delete<c_gui_saved_film_subitem_datasource>(recent_films);
	//}
	//
	//c_gui_screen_widget::add_datasource(recent_films);
}

bool __cdecl parse_xml_lobby_theater_film(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B029C0, parse_xml_lobby_theater_film, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_theater_film_length(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	return INVOKE(0x00B02A60, parse_xml_lobby_theater_film_length, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_xml_lobby_theater_game(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	const s_saved_film_description* film = user_interface_game_settings_get_film();
	if (!film)
	{
		return false;
	}

	if (film->category == _saved_film_category_none || film->category == _saved_film_category_invalid)
	{
		return false;
	}

	return c_gui_pregame_setup_manager::get()->get_selected_item_title(buffer, buffer_length, _gui_selection_type_game_variant);
}

bool __cdecl parse_xml_lobby_theater_map(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	//return INVOKE(0x00B02B20, parse_xml_lobby_theater_map, this_ptr, buffer, buffer_length);

	const s_saved_film_description* film = user_interface_game_settings_get_film();
	if (!film)
	{
		return false;
	}

	if (film->category == _saved_film_category_none || film->category == _saved_film_category_invalid)
	{
		return false;
	}

	return c_gui_pregame_setup_manager::get()->get_selected_item_title(buffer, buffer_length, _gui_selection_type_map);
}

bool c_gui_screen_pregame_lobby_theater::team_switching_allowed()
{
	//return INVOKE_CLASS_MEMBER(0x00B02B60, c_gui_screen_pregame_lobby_theater, team_switching_allowed);

	return false;
}

void c_gui_screen_pregame_lobby_theater::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00B02B70, c_gui_screen_pregame_lobby_theater, update, current_milliseconds);

	c_gui_screen_pregame_lobby::update(current_milliseconds);

	c_gui_widget::set_child_visible(_gui_bitmap, STRING_ID(gui, gametype_image), false);
	c_gui_widget::set_child_visible(_gui_bitmap, STRING_ID(gui, difficulty_image), false);
	c_gui_widget::set_child_visible(_gui_bitmap, STRING_ID(gui, map_image), false);
	c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, game_name), false);
	c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, theater_no_film), false);
	c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, theater_invalid_film), false);

	if (user_interface_squad_exists() && user_interface_get_session_game_mode() == _session_game_mode_pregame)
	{
		const s_saved_film_description* film = user_interface_game_settings_get_film();
		if (film)
		{
			if (film->category == _saved_film_category_none)
			{
				c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, theater_no_film), true);
			}
			else if (film->category == _saved_film_category_invalid)
			{
				c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, theater_invalid_film), true);
			}
			else
			{
				if (film->campaign_id == _campaign_id_none)
				{
					c_gui_bitmap_widget* gametype_image_bitmap_widget = c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, gametype_image));
					const c_game_variant* current_variant = c_gui_screen_pregame_lobby::get_current_variant();
					if (gametype_image_bitmap_widget && current_variant)
					{
						gametype_image_bitmap_widget->set_visible(true);
						gametype_image_bitmap_widget->set_sprite_frame(current_variant->get_game_engine_index());
					}
				}
				else
				{
					c_gui_bitmap_widget* difficulty_image_bitmap_widget = c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, difficulty_image));
					if (difficulty_image_bitmap_widget)
					{
						difficulty_image_bitmap_widget->set_visible(true);
						difficulty_image_bitmap_widget->set_sprite_frame(user_interface_game_settings_get_campaign_difficulty());
					}
				}

				c_gui_widget::set_child_visible(_gui_bitmap, STRING_ID(gui, map_image), true);
				c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, game_name), true);
			}
		}
	}

	if (m_recent_film_set)
	{
		return;
	}

	const s_saved_film_description* film = user_interface_game_settings_get_film();
	if (film && film->category != _saved_film_category_none)
	{
		m_recent_film_set = true;
		return;
	}

	c_gui_data* data = c_gui_screen_widget::get_data(STRING_ID(gui, recent_films), NULL);
	if (!data || data->is_busy())
	{
		return;
	}

	int32 element_handle = data->get_first_element_handle();
	if (element_handle != NONE)
	{
		c_gui_map_selected_item* map_selected_item = (c_gui_map_selected_item*)data->get_gui_selected_item(element_handle);
		if (map_selected_item)
		{
			c_gui_pregame_setup_manager::get()->set_selected_item(c_gui_widget::get_arbitrary_responding_controller(), map_selected_item, true, NONE);
		}
	}

	m_recent_film_set = true;
}


