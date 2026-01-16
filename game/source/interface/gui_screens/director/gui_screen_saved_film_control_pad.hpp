#pragma once

#include "interface/c_gui_screen_widget.hpp"

class c_saved_film_control_pad :
	public c_gui_screen_widget
{
public:
	bool __thiscall handle_list_item_chosen_(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource);
	void __thiscall update_(uns32 current_milliseconds);
	void __thiscall update_enabled_menu_items_();

public:
	void update_enabled_menu_items();

public:
	int32 m_previous_snippet_state;
};
//COMPILE_ASSERT(sizeof(c_saved_film_control_pad) == 0x1AA8);
//COMPILE_ASSERT(sizeof(c_saved_film_control_pad) == sizeof(c_gui_screen_widget) + 0x8);

