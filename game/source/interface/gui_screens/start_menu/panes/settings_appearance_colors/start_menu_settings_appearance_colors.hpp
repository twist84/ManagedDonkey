#pragma once

#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

class c_start_menu_settings_appearance_colors :
	public c_start_menu_pane_screen_widget
{
public:
	bool __thiscall handle_controller_input_message_(const c_controller_input_message* input_message);
	void __thiscall initialize_();
	void set_color_focused_list_item(int32 name, e_player_color_index player_color_index);
	void __thiscall set_color_values_from_profile();
};
COMPILE_ASSERT(sizeof(c_start_menu_settings_appearance_colors) == sizeof(c_start_menu_pane_screen_widget));

extern bool __cdecl parse_xml_armor1(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_armor2(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_armor3(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_color(c_gui_screen_widget* screen_widget, wchar_t* buffer, int32 buffer_length, int32 name);
extern bool __cdecl parse_xml_emblem1(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_emblem2(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_emblem3(void* this_ptr, wchar_t* buffer, int32 buffer_length);

