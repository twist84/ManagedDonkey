#pragma once

#include "interface/c_gui_screen_widget.hpp"

enum e_gui_error_category
{
	_gui_error_category_default = 0,
	_gui_error_category_networking,
	_gui_error_category_storage,
	_gui_error_category_controller,

	k_gui_error_category_count
};

struct s_gui_error
{
	short error_category;
	long error_name;
};
static_assert(sizeof(s_gui_error) == 0x8);

enum e_controller_index;
struct c_error_dialog_screen_widget :
	public c_gui_screen_widget
{
public:

//protected:
	e_controller_index m_controller_index;
	s_gui_error m_error_condition;
};
static_assert(sizeof(c_error_dialog_screen_widget) == sizeof(c_gui_screen_widget) + 0xC);

