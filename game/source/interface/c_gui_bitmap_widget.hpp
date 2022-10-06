#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"

struct c_gui_bitmap_widget : c_gui_widget
{
	byte __dataDC[0x8];
	long m_sprite_frame;
	char __dataE8[0x8];
	c_string_id m_name;
	byte __dataF4[0x2C];
	long m_bitmap_definition_index;
	byte __data124[0x14];
};
static_assert(sizeof(c_gui_bitmap_widget) == 0x138);
