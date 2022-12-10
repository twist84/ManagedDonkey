#pragma once

#include "interface/c_gui_widget.hpp"

struct c_gui_list_item_widget : public c_gui_widget
{
protected:
	byte __dataDC[0x40];
	bool __unknown11C;
	wchar_t __unknown11E[200];
};
static_assert(sizeof(c_gui_list_item_widget) == sizeof(c_gui_widget) + 0x1D4);

