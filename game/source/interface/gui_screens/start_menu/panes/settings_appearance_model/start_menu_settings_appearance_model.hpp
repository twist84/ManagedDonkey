#pragma once

#include "cseries/cseries.hpp"
#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

struct c_start_menu_settings_appearance_model :
	public c_start_menu_pane_screen_widget
{
	byte __data2168[0x8];
};
static_assert(sizeof(c_start_menu_settings_appearance_model) == sizeof(c_start_menu_pane_screen_widget) + 0x8);

extern bool __cdecl start_menu_pane_parse_part_count_current(void* this_ptr, wchar_t* buffer, long buffer_size);
extern bool __cdecl start_menu_pane_parse_part_count_total(void* this_ptr, wchar_t* buffer, long buffer_size);
extern bool __cdecl start_menu_pane_parse_part_description(void* this_ptr, wchar_t* buffer, long buffer_size);

