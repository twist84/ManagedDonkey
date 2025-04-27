#pragma once

#include "interface/c_gui_screen_widget.hpp"

struct s_gui_error
{
	int16 error_category;
	int32 error_name;
};
static_assert(sizeof(s_gui_error) == 0x8);

struct c_error_dialog_screen_widget :
	public c_gui_screen_widget
{
public:
	c_error_dialog_screen_widget(int32 name) :
		c_gui_screen_widget(name)
	{
		DECLFUNC(0x00AE6DF0, c_error_dialog_screen_widget*, __thiscall, c_error_dialog_screen_widget*, int32)(this, name);
	}

//protected:
	e_controller_index m_controller_index;
	s_gui_error m_error_condition;
};
static_assert(sizeof(c_error_dialog_screen_widget) == sizeof(c_gui_screen_widget) + 0xC);

