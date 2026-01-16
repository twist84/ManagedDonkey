#pragma once

#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

class c_start_menu_settings :
	public c_start_menu_pane_screen_widget
{
public:
	virtual ~c_start_menu_settings() = default;
	virtual bool allow_pane_tab_change();
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource);
	virtual void initialize();
	virtual void update(uns32 current_milliseconds);

public:
	c_start_menu_settings(int32 name);
};
COMPILE_ASSERT(sizeof(c_start_menu_settings) == 0x2168);
COMPILE_ASSERT(sizeof(c_start_menu_settings) == sizeof(c_start_menu_pane_screen_widget));

