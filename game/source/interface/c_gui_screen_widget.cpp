#include "interface/c_gui_screen_widget.hpp"

#include "interface/user_interface_data.hpp"
#include "interface/user_interface_text_parser.hpp"

c_gui_data* c_gui_screen_widget::get_data(long name, long* datasource_index)
{
	//return DECLFUNC(0x00AB0DB0, c_gui_data*, __thiscall, c_gui_screen_widget*, long, long*)(this, name, datasource_index);

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
	return DECLFUNC(0x00AB0F40, c_gui_widget*, __thiscall, c_gui_screen_widget*)(this);
}

e_window_index c_gui_screen_widget::get_render_window()
{
	return DECLFUNC(0x00AB0FF0, e_window_index, __thiscall, c_gui_screen_widget*)(this);
}

void c_gui_screen_widget::add_game_tag_parser(c_game_tag_parser* parser)
{
	//DECLFUNC(0x00AB0800, void, __thiscall, c_gui_screen_widget*, c_game_tag_parser*)(this, parser);

	if (parser)
	{
		if (m_game_tag_parser_count < NUMBEROF(m_game_tag_parser))
		{
			user_interface_text_parser_register(parser, m_screen_index);
			m_game_tag_parser[m_game_tag_parser_count++] = parser;
		}
	}
}

void c_gui_screen_widget::transfer_focus_to_list(c_gui_list_widget* list_widget, long item_index, bool a3, bool a4)
{
	DECLFUNC(0x00AB2640, void, __thiscall, c_gui_screen_widget*, c_gui_list_widget*, long, bool, bool)(this, list_widget, item_index, a3, a4);
}

void c_gui_screen_widget::add_datasource(c_gui_data* datasource)
{
	m_datasource[m_datasource_count++] = datasource;
}