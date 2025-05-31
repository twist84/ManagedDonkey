#pragma once

#include "cseries/cseries.hpp"
#include "interface/debug_menu/debug_menu_item_numbered.hpp"

class c_debug_menu_item_hs_command :
	public c_debug_menu_item_numbered
{
public:
	virtual void notify_selected() override;
	virtual const real_argb_color* get_enabled_color() override;

	c_debug_menu_item_hs_command(c_debug_menu* menu, const char* name, const char* command);

protected:
	char* m_command;
};

