#pragma once

#include "cseries/cseries.hpp"

struct c_gui_widget_vtbl;
struct c_gui_widget
{
	void* __vftable /*VFT*/;
	byte __data[0xD8];
};
static_assert(sizeof(c_gui_widget) == 0xDC);
