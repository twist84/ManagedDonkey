#include "interface/c_gui_list_widget.hpp"

c_gui_data* __cdecl c_gui_list_widget::get_data()
{
	return DECLFUNC(0x00B14FE0, c_gui_data*, __thiscall, c_gui_list_widget*)(this);
}

void __cdecl c_gui_list_widget::set_selectable_item_cap_count(long selectable_item_cap_count)
{
	m_selectable_item_cap_count = selectable_item_cap_count;
}

