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
	s_tag_block list_blocks;
	s_tag_block text_blocks;
	s_tag_block bitmap_blocks;
	s_tag_block model_blocks;
};
COMPILE_ASSERT(sizeof(s_group_widget_definition) == 0x5C);
COMPILE_ASSERT(sizeof(s_group_widget_definition) == sizeof(s_core_widget_definition) + 0x30);

struct s_group_widget_block
{
	s_tag_reference widget_template_reference;
	s_group_widget_definition override_definition;
};
COMPILE_ASSERT(sizeof(s_group_widget_block) == 0x6C);

struct s_runtime_group_widget_definition :
	s_runtime_core_widget_definition
{
	s_tag_block list;
	s_tag_block text_items;
	s_tag_block bitmaps;
	s_tag_block models;
	int32 string_list_reference_index;
};
COMPILE_ASSERT(sizeof(s_runtime_group_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x34);

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
COMPILE_ASSERT(sizeof(c_gui_group_widget) == 0x150);
COMPILE_ASSERT(sizeof(c_gui_group_widget) == sizeof(c_gui_widget) + 0x70);
COMPILE_ASSERT(0x0E0 == OFFSETOF(c_gui_group_widget, m_core_definition));
COMPILE_ASSERT(0x148 == OFFSETOF(c_gui_group_widget, m_dispose_as_display_group));
COMPILE_ASSERT(0x14C == OFFSETOF(c_gui_group_widget, __unknown14C));

class c_gui_button_key_widget :
	public c_gui_group_widget
{
};
COMPILE_ASSERT(sizeof(c_gui_button_key_widget) == 0x150);
COMPILE_ASSERT(sizeof(c_gui_button_key_widget) == sizeof(c_gui_group_widget));

