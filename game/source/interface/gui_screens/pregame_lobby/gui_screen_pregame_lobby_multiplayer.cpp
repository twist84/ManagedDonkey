#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_multiplayer.hpp"

#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/gui_pregame_setup_manager.hpp"
#include "interface/gui_selected_items.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B01250, c_gui_screen_pregame_lobby_multiplayer, handle_list_item_chosen_);

bool __thiscall c_gui_screen_pregame_lobby_multiplayer::handle_list_item_chosen_(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return c_gui_screen_pregame_lobby_multiplayer::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

c_gui_screen_pregame_lobby_multiplayer::c_gui_screen_pregame_lobby_multiplayer(int32 name) :
	c_gui_screen_pregame_lobby(name)
{
	//DECLFUNC(0x00B01130, void, __thiscall, c_gui_screen_pregame_lobby_multiplayer*, int32)(this, name);
}

//.text:00B01150 ; public: virtual c_gui_screen_pregame_lobby_multiplayer::~c_gui_screen_pregame_lobby_multiplayer()

//.text:00B011A0 ; public: virtual void* c_gui_screen_pregame_lobby_multiplayer::`scalar deleting destructor'(unsigned int)
c_gui_screen_pregame_lobby_multiplayer::~c_gui_screen_pregame_lobby_multiplayer()
{
	//DECLFUNC(0x00B011A0, void, __thiscall, c_gui_screen_pregame_lobby_multiplayer*)(this);
}

e_gui_game_mode c_gui_screen_pregame_lobby_multiplayer::get_gui_game_mode()
{
	//return INVOKE_CLASS_MEMBER(0x00B01210, c_gui_screen_pregame_lobby_multiplayer, get_gui_game_mode);

	return _ui_game_mode_multiplayer;
}

int32 c_gui_screen_pregame_lobby_multiplayer::get_lobby_header()
{
	//return INVOKE_CLASS_MEMBER(0x00B01220, c_gui_screen_pregame_lobby_multiplayer, get_lobby_header);

	return STRING_ID(gui, header_multiplayer);
}

int32 c_gui_screen_pregame_lobby_multiplayer::get_lobby_title()
{
	//return INVOKE_CLASS_MEMBER(0x00B01230, c_gui_screen_pregame_lobby_multiplayer, get_lobby_title);

	return STRING_ID(gui, title_multiplayer);
}

int32 c_gui_screen_pregame_lobby_multiplayer::get_start_button_name()
{
	//return INVOKE_CLASS_MEMBER(0x00B01240, c_gui_screen_pregame_lobby_multiplayer, get_start_button_name);

	return STRING_ID(gui, start_multiplayer);
}

bool c_gui_screen_pregame_lobby_multiplayer::handle_controller_input_message(const c_controller_input_message* message)
{
	if (message->get_event_type() == _event_type_button_press && message->get_component() == _controller_component_button_x)
	{
		if (user_interface_squad_in_or_after_countdown())
		{
			user_interface_squad_stop_countdown_timer(message->get_controller(), -1, false);
		}
		else
		{
			c_gui_screen_pregame_lobby::load_game_variant_editing_screen(message->get_controller());
		}

		return true;
	}

	return c_gui_screen_pregame_lobby::handle_controller_input_message(message);
}

bool c_gui_screen_pregame_lobby_multiplayer::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	//INVOKE_CLASS_MEMBER(0x00B01250, c_gui_screen_pregame_lobby_multiplayer, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	if (list_name == STRING_ID(gui, lobby_list))
	{
		int32 target_name = _string_id_invalid;
		int32 element_handle = list_item_widget->get_element_handle();
		if (datasource->get_string_id_value(element_handle, STRING_ID(gui, target), &target_name))
		{
			e_gui_selected_item_type selection_type = _gui_selection_type_none;
			switch (target_name)
			{
			case STRING_ID(global, variant):
			{
				selection_type = _gui_selection_type_game_variant;
			}
			break;
			case STRING_ID(global, map):
			{
				selection_type = _gui_selection_type_map;
			}
			break;
			}

			if (selection_type != _gui_selection_type_none)
			{
				if (c_load_pregame_selection_screen_message* pregame_selection_screen_message = UI_MALLOC(c_load_pregame_selection_screen_message,
					message->get_controller(),
					c_gui_screen_widget::get_render_window(),
					m_name,
					selection_type))
				{
					pregame_selection_screen_message->set_parent_screen_index(m_screen_index);
					user_interface_messaging_post(pregame_selection_screen_message);
				}
				return true;
			}
		}
		else if (datasource->get_string_id_value(element_handle, STRING_ID(global, name), &target_name) && target_name == STRING_ID(gui, advanced_options))
		{
			c_gui_screen_pregame_lobby::load_game_variant_editing_screen(message->get_controller());
			return true;
		}
	}

	return c_gui_screen_pregame_lobby::handle_list_item_chosen(message, list_name, list_item_widget, datasource);
}

void c_gui_screen_pregame_lobby_multiplayer::initialize()
{
	//INVOKE_CLASS_MEMBER(0x00B013B0, c_gui_screen_pregame_lobby_multiplayer, initialize);

	c_gui_screen_pregame_lobby::initialize();

	m_initial_focused_widget = STRING_ID(gui, lobby_list);
	m_initial_focused_widget_element_handle = 4;

	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-multiplayer-game", this, parse_xml_lobby_multiplayer_game));
	c_gui_screen_widget::add_game_tag_parser(UI_MALLOC(c_magic_string_game_tag_parser, L"<lobby-multiplayer-map", this, parse_xml_lobby_multiplayer_map));
}

bool __cdecl parse_xml_lobby_multiplayer_game(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	//return INVOKE(0x00B01470, parse_xml_lobby_multiplayer_game, this_ptr, buffer, buffer_length);

	return c_gui_pregame_setup_manager::get()->get_selected_item_title(buffer, buffer_length, _gui_selection_type_game_variant);
}

bool __cdecl parse_xml_lobby_multiplayer_map(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	//return INVOKE(0x00B01490, parse_xml_lobby_multiplayer_map, this_ptr, buffer, buffer_length);

	return c_gui_pregame_setup_manager::get()->get_selected_item_title(buffer, buffer_length, _gui_selection_type_map);
}

void c_gui_screen_pregame_lobby_multiplayer::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00B014B0, c_gui_screen_pregame_lobby_multiplayer, update, current_milliseconds);

	c_gui_screen_pregame_lobby::update(current_milliseconds);

	if (user_interface_squad_exists() && user_interface_get_session_game_mode() == _session_game_mode_pregame)
	{
		c_gui_bitmap_widget* gametype_image_bitmap_widget = c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, gametype_image));
		if (gametype_image_bitmap_widget)
		{
			const c_game_variant* current_variant = c_gui_screen_pregame_lobby::get_current_variant();
			if (current_variant)
			{
				gametype_image_bitmap_widget->set_sprite_frame(current_variant->get_game_engine_index());
			}
		}
	}
}

