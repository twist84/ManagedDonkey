#include "interface/c_gui_screen_widget.hpp"

#include "interface/user_interface_data.hpp"
#include "interface/user_interface_text_parser.hpp"

c_gui_data* c_gui_screen_widget::get_data(long name, long* datasource_index)
{
	//return INVOKE_CLASS_MEMBER(0x00AB0DB0, c_gui_screen_widget, get_data, name, datasource_index);

	if (datasource_index)
		*datasource_index = -1;

	if (m_datasource_count <= 0)
		return nullptr;

	long index = 0;
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

e_window_index c_gui_screen_widget::get_render_window()
{
	return INVOKE_CLASS_MEMBER(0x00AB0FF0, c_gui_screen_widget, get_render_window);
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

void c_gui_screen_widget::transfer_focus(c_gui_widget* widget)
{
	INVOKE_CLASS_MEMBER(0x00AB25F0, c_gui_screen_widget, transfer_focus, widget);
}

void c_gui_screen_widget::transfer_focus_to_list(c_gui_list_widget* list_widget, long item_index, bool a3, bool a4)
{
	INVOKE_CLASS_MEMBER(0x00AB2640, c_gui_screen_widget, transfer_focus_to_list, list_widget, item_index, a3, a4);
}

void c_gui_screen_widget::add_datasource(c_gui_data* datasource)
{
	m_datasource[m_datasource_count++] = datasource;
}