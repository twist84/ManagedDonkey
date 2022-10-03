#pragma once

#include "cseries/cseries.hpp"
#include "interface/gui_animation.hpp"
#include "tag_files/tag_groups.hpp"

enum e_core_widget_definition_flags
{
	_core_widget_definition_flag_do_not_apply_old_content_upscaling_bit = 0,
	_core_widget_definition_flag_override_template_flags_bit,
	_core_widget_definition_flag_enable_animation_debugging_bit,

	k_core_widget_definition_flag_count
};

enum e_widget_positioning
{
	_widget_positioning_unused = 0,
	_widget_positioning_centered,
	_widget_positioning_top_edge,
	_widget_positioning_bottom_edge,
	_widget_positioning_left_edge,
	_widget_positioning_right_edge,
	_widget_positioning_top_left_corner,
	_widget_positioning_top_right_corner,
	_widget_positioning_bottom_right_corner,
	_widget_positioning_bottom_left_corner,

	k_widget_positioning_count
};

struct s_core_widget_definition
{
	// e_core_widget_definition_flags
	dword_flags flags;

	// identification only
	c_string_id name;

	// scaled positioning & scaled anchoring
	c_enum<e_widget_positioning, short, k_widget_positioning_count> scaled_positioning;
	short render_depth_bias;

	// bounds 720p & bounds 16x9 (1152x640)
	// bounds 480i & bounds 4x3 (640x480)
	short_rectangle2d ui_bounds[2];

	c_typed_tag_reference<'wacd'> animation_collection;
};
static_assert(sizeof(s_core_widget_definition) == 0x2C);

struct s_runtime_core_widget_definition
{
	// e_core_widget_definition_flags
	dword_flags flags;

	// identification only
	c_string_id name;

	// scaled positioning & scaled anchoring
	c_enum<e_widget_positioning, long, k_widget_positioning_count> scaled_positioning;
	long render_depth_bias;

	// bounds 720p & bounds 16x9 (1152x640)
	// bounds 480i & bounds 4x3 (640x480)
	real_rectangle2d ui_bounds[2];

	long animation_collection_index;
};
static_assert(sizeof(s_runtime_core_widget_definition) == 0x34);

enum e_gui_widget_type
{
	_gui_widget_type_text_widget = 0,
	_gui_widget_type_bitmap_widget,
	_gui_widget_type_model_widget,
	_gui_widget_type_group_widget,
	_gui_widget_type_button_key_widget,
	_gui_widget_type_list_item_widget,

	// unused?
	_gui_widget_type_unknown_widget,

	_gui_widget_type_list_widget,
	_gui_widget_type_screen_widget,

	k_gui_widget_type_count
};

struct c_gui_widget
{
	void* __vftable;

	long __unknown4;
	c_enum<e_gui_widget_type, long, k_gui_widget_type_count> m_type;

	bool m_visible;
	bool m_enabled;
	bool __unknownE;
	bool m_use_alternate_ambient_state;

	struct s_unknown
	{
		c_gui_widget* __unknown0;
		c_gui_widget* __unknown4;
		c_gui_widget* __unknown8;
	};

	s_unknown m_parent;
	s_unknown m_children;
	s_unknown m_next;
	s_unknown m_previous;

	//c_gui_widget* m_parent;
	//c_gui_widget* m_children;
	//c_gui_widget* m_next;
	//c_gui_widget* m_previous;

	c_string_id m_name;
	real __unknown44;
	dword_flags m_flags;
	long __unknown4C;
	s_animation_transform m_full_animation_state;
	long __unknownD8;
};
static_assert(sizeof(c_gui_widget) == 0xDC);
