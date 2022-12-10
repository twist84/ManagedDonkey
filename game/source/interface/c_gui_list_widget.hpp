#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"

struct c_gui_data;
struct c_gui_list_widget : c_gui_widget
{
public:
	c_gui_data* __cdecl get_data();

protected:
	byte __dataDC[0x70];
	long m_selectable_item_cap_count;
	byte __data150[0x8];
};
static_assert(sizeof(c_gui_list_widget) == sizeof(c_gui_widget) + 0x7C);

