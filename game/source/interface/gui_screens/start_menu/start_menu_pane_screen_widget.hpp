#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "networking/online/online_service_record.hpp"

struct c_start_menu_pane_screen_widget :
	public c_gui_screen_widget
{
	byte __data1AA0[0x18];
	s_service_record_identity m_identity;
	byte __data2158[0x8];
	long m_automatic_button_key;
	byte __data2164[0x4];
};
static_assert(sizeof(c_start_menu_pane_screen_widget) == sizeof(c_gui_screen_widget) + 0x6C8);

