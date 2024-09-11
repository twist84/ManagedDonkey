#include "interface/c_gui_list_widget.hpp"

c_gui_data* c_gui_list_widget::get_data()
{
	return DECLFUNC(0x00B14FE0, c_gui_data*, __thiscall, c_gui_list_widget*)(this);
}

bool c_gui_list_widget::set_focused_item_index(long focused_item_index, bool a2)
{
	return DECLFUNC(0x00B162E0, bool, __thiscall, c_gui_list_widget*, long, bool)(this, focused_item_index, a2);
}

void c_gui_list_widget::set_selectable_item_cap_count(long selectable_item_cap_count)
{
	m_selectable_item_cap_count = selectable_item_cap_count;
}

