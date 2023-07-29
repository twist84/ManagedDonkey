#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"

enum e_list_widget_definition_flags
{
	_list_widget_definition_flag_submenu_list_bit = k_core_widget_definition_flag_count,
	_list_widget_definition_flag_horizontal_list_bit,
	_list_widget_definition_flag_list_wraps_bit,
	_list_widget_definition_flag_selection_visible_without_focus_bit,
	_list_widget_definition_flag_non_interactive_bit,

	k_list_widget_definition_flag_count
};

struct s_list_widget_definition : s_core_widget_definition
{
	c_string_id datasource_name;
	s_tag_reference skin;
	long rows;
	s_tag_block items;
	s_tag_reference prev_indicator_bitmap;
	s_tag_reference next_indicator_bitmap;
};
static_assert(sizeof(s_list_widget_definition) == sizeof(s_core_widget_definition) + 0x44);

struct s_runtime_list_widget_definition : s_runtime_core_widget_definition
{
	c_string_id datasource_name;
	long skin_index;
	long rows;
	s_tag_block items;
	long prev_indicator_bitmap_reference_index;
	long next_indicator_bitmap_reference_index;
};
static_assert(sizeof(s_runtime_list_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x20);

struct c_gui_data;
struct c_gui_list_widget : public c_gui_widget
{
public:
	c_gui_data* __cdecl get_data();
	void __cdecl set_selectable_item_cap_count(long selectable_item_cap_count);

protected:
	long __unknownDC;
	s_runtime_list_widget_definition m_core_definition;
	c_string_id m_datasource_name;
	c_gui_widget* m_prev_indicator;
	c_gui_widget* m_next_indicator;
	long m_focused_item_index;
	long __unknown144;
	long m_scroll_position;
	long m_selectable_item_cap_count;
	c_gui_widget* m_submenu_item;
	bool m_is_submenu;
	bool m_is_submenu_that_needs_disposal;
};
static_assert(sizeof(c_gui_list_widget) == sizeof(c_gui_widget) + 0x7C);

