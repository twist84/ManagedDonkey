#pragma once

#include "interface/c_gui_widget.hpp"

struct s_list_item_widget_definition :
	s_core_widget_definition
{
	c_string_id item_label;
};
static_assert(sizeof(s_list_item_widget_definition) == sizeof(s_core_widget_definition) + 0x4);

struct s_runtime_list_item_widget_definition :
	s_runtime_core_widget_definition
{
	c_string_id item_label;
};
static_assert(sizeof(s_runtime_list_item_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x4);

struct c_gui_list_item_widget :
	public c_gui_widget
{
public:
	long get_list_item_index();

//protected:
	long __unknownDC;
	s_runtime_list_item_widget_definition m_core_definition;
	long m_list_item_index;
	bool __unknown11C;
	c_static_wchar_string<200> __string11E;
};
static_assert(sizeof(c_gui_list_item_widget) == sizeof(c_gui_widget) + 0x1D4);

