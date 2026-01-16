#pragma once

#include "interface/c_gui_screen_widget.hpp"

class c_gui_screen_pregame_switch_lobby :
	public c_gui_screen_widget
{
public:
	virtual ~c_gui_screen_pregame_switch_lobby() = default;
	virtual void post_initialize() override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;

public:
	c_gui_screen_pregame_switch_lobby(int32 name);
};
COMPILE_ASSERT(sizeof(c_gui_screen_pregame_switch_lobby) == 0x1AA0);
COMPILE_ASSERT(sizeof(c_gui_screen_pregame_switch_lobby) == sizeof(c_gui_screen_widget));

