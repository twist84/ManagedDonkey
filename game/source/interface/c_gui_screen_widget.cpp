#include "interface/c_gui_screen_widget.hpp"

#include "interface/user_interface_data.hpp"

c_gui_data* __cdecl c_gui_screen_widget::get_data(long name, long* datasource_index)
{
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

e_window_index __cdecl c_gui_screen_widget::get_render_window()
{
	return INVOKE_CLASS_MEMBER(0x00AB0FF0, c_gui_screen_widget::get_render_window);
}

void c_gui_screen_widget::add_datasource(c_gui_data* datasource)
{
	m_datasource[m_datasource_count++] = datasource;
}