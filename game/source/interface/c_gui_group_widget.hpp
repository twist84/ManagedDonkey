#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"
#include "tag_files/tag_groups.hpp"

enum e_group_widget_definition_flags
{
	_group_widget_definition_flag_not_loaded_upon_initialization_bit = k_core_widget_definition_flags,
	_group_widget_definition_flag_bit4,
	_group_widget_definition_flag_bit5,

	k_group_widget_definition_flags
};

struct s_group_widget_definition : s_core_widget_definition
{
	s_tag_block list;
	s_tag_block text_items;
	s_tag_block bitmaps;
	s_tag_block models;
};
static_assert(sizeof(s_group_widget_definition) == sizeof(s_core_widget_definition) + 0x30);

struct s_runtime_group_widget_definition : s_runtime_core_widget_definition
{
	s_tag_block list;
	s_tag_block text_items;
	s_tag_block bitmaps;
	s_tag_block models;
	long string_list_reference_index;
};
static_assert(sizeof(s_runtime_group_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x34);

struct c_gui_group_widget : public c_gui_widget
{
	long __unknownDC;
	s_runtime_group_widget_definition m_core_definition;
	bool m_dispose_as_display_group;
	long __unknown14C;
};
static_assert(sizeof(c_gui_group_widget) == sizeof(c_gui_widget) + 0x74);

struct c_gui_button_key_widget : public c_gui_group_widget
{
};
static_assert(sizeof(c_gui_button_key_widget) == sizeof(c_gui_group_widget));

