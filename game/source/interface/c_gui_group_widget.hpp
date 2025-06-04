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

struct s_group_widget_definition :
	s_core_widget_definition
{
	s_tag_block list;
	s_tag_block text_items;
	s_tag_block bitmaps;
	s_tag_block models;
};
static_assert(sizeof(s_group_widget_definition) == sizeof(s_core_widget_definition) + 0x30);

struct s_runtime_group_widget_definition :
	s_runtime_core_widget_definition
{
	s_tag_block list;
	s_tag_block text_items;
	s_tag_block bitmaps;
	s_tag_block models;
	int32 string_list_reference_index;
};
static_assert(sizeof(s_runtime_group_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x34);

class c_gui_group_widget :
	public c_gui_widget
{
public:
	void set_dispose_as_display_group(bool dispose);

//protected:
	s_runtime_group_widget_definition m_core_definition;
	bool m_dispose_as_display_group;
	int32 __unknown14C;
};
static_assert(sizeof(c_gui_group_widget) == 0x150);
static_assert(sizeof(c_gui_group_widget) == sizeof(c_gui_widget) + 0x70);
static_assert(0x0E0 == OFFSETOF(c_gui_group_widget, m_core_definition));
static_assert(0x148 == OFFSETOF(c_gui_group_widget, m_dispose_as_display_group));
static_assert(0x14C == OFFSETOF(c_gui_group_widget, __unknown14C));

class c_gui_button_key_widget :
	public c_gui_group_widget
{
};
static_assert(sizeof(c_gui_button_key_widget) == sizeof(c_gui_group_widget));

