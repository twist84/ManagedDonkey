#pragma once

#include "cseries/cseries.hpp"
#include "interface/debug_menu/debug_menu_item_numbered.hpp"

class c_debug_menu_item_hs_command :
	public c_debug_menu_item_numbered
{
public:
	virtual void notify_selected() override;
	virtual real_argb_color const* get_enabled_color() override;

	c_debug_menu_item_hs_command(c_debug_menu* menu, char const* name, char const* command);

protected:
	char* m_command;
};

