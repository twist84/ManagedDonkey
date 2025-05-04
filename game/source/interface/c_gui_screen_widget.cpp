#include "interface/c_gui_screen_widget.hpp"

#include "interface/user_interface_data.hpp"
#include "interface/user_interface_text_parser.hpp"

void c_gui_screen_widget::add_datasource(c_gui_data* datasource)
{
	m_datasource[m_datasource_count++] = datasource;
}

void c_gui_screen_widget::add_game_tag_parser(c_game_tag_parser* parser)
{
	//INVOKE_CLASS_MEMBER(0x00AB0800, c_gui_screen_widget, add_game_tag_parser, parser);

	if (parser)
	{
		if (m_game_tag_parser_count < NUMBEROF(m_game_tag_parsers))
		{
			user_interface_text_parser_register(parser, m_screen_index);
			m_game_tag_parsers[m_game_tag_parser_count++] = parser;
		}
	}
}

c_gui_data* c_gui_screen_widget::get_data(int32 name, int32* datasource_index)
{
	//return INVOKE_CLASS_MEMBER(0x00AB0DB0, c_gui_screen_widget, get_data, name, datasource_index);

	if (datasource_index)
		*datasource_index = NONE;

	if (m_datasource_count <= 0)
		return nullptr;

	int32 index = 0;
	for (c_gui_data** datasource = m_datasource; (*datasource)->get_name().get_value() != name; datasource++)
	{
		if (++index >= m_datasource_count)
			return nullptr;
	}

	if (datasource_index)
		*datasource_index = index;

	return m_datasource[index];
}

c_gui_widget* c_gui_screen_widget::get_focused_widget()
{
	return INVOKE_CLASS_MEMBER(0x00AB0F40, c_gui_screen_widget, get_focused_widget);
}

e_gui_game_mode c_gui_screen_widget::get_gui_game_mode()
{
	return INVOKE_CLASS_MEMBER((uns32)__vftable->__funcs[60], c_gui_screen_widget, get_gui_game_mode);
}

e_window_index c_gui_screen_widget::get_render_window()
{
	return INVOKE_CLASS_MEMBER(0x00AB0FF0, c_gui_screen_widget, get_render_window);
}

bool c_gui_screen_widget::handle_list_item_chosen(c_controller_input_message const* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00AB14C0, c_gui_screen_widget, handle_list_item_chosen, message, list_name, list_item_widget, datasource);
	//return INVOKE_CLASS_MEMBER((uns32)__vftable->__funcs[49], c_gui_screen_widget, handle_list_item_chosen, message, list_name, list_item_widget, datasource);

	return false;
}

void c_gui_screen_widget::play_sound(e_user_interface_sound_effect sound_effect)
{
	INVOKE_CLASS_MEMBER(0x00AB18A0, c_gui_screen_widget, play_sound, sound_effect);

	//user_interface_play_sound(sound_effect, m_definition.sound_overrides_index);
}

bool c_gui_screen_widget::running_in_codeless_mode()
{
	return m_running_in_codeless_mode;
}

void c_gui_screen_widget::transfer_focus(c_gui_widget* widget)
{
	INVOKE_CLASS_MEMBER(0x00AB25F0, c_gui_screen_widget, transfer_focus, widget);
}

void c_gui_screen_widget::transfer_focus_to_list(c_gui_list_widget* list_widget, int32 element_handle, bool play_received_animation, bool play_lost_animation)
{
	INVOKE_CLASS_MEMBER(0x00AB2640, c_gui_screen_widget, transfer_focus_to_list, list_widget, element_handle, play_received_animation, play_lost_animation);
}

