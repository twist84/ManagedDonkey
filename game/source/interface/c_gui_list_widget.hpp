#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"

enum e_list_widget_definition_flags
{
	_list_widget_definition_flag_submenu_list_bit = k_core_widget_definition_flags,
	_list_widget_definition_flag_horizontal_list_bit,
	_list_widget_definition_flag_list_wraps_bit,
	_list_widget_definition_flag_selection_visible_without_focus_bit,
	_list_widget_definition_flag_non_interactive_bit,

	k_list_widget_definition_flags
};

struct s_list_widget_definition :
	s_core_widget_definition
{
	c_string_id datasource_name;
	s_tag_reference skin;
	int32 rows;
	s_tag_block items;
	s_tag_reference prev_indicator_bitmap;
	s_tag_reference next_indicator_bitmap;
};
static_assert(sizeof(s_list_widget_definition) == sizeof(s_core_widget_definition) + 0x44);

struct s_runtime_list_widget_definition :
	s_runtime_core_widget_definition
{
	c_string_id datasource_name;
	int32 skin_index;
	int32 rows;
	s_tag_block items;
	int32 prev_indicator_bitmap_reference_index;
	int32 next_indicator_bitmap_reference_index;
};
static_assert(sizeof(s_runtime_list_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x20);

struct c_gui_data;
struct c_gui_list_widget :
	public c_gui_widget
{
public:
	enum e_list_scroll_direction
	{
		_list_scroll_direction_none = 0,
		_list_scroll_direction_forward,
		_list_scroll_direction_backward,
		_list_scroll_direction_page_next,
		_list_scroll_direction_page_previous,
	};

public:
	virtual ~c_gui_list_widget();
	virtual void post_initialize() override;
	virtual int32 get_datasource_index() override;
	virtual s_runtime_core_widget_definition* get_core_definition() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual void update_render_state(uns32 current_milliseconds) override;
	virtual bool handle_tab(c_controller_input_message const* message) override;
	virtual void initialize(s_list_widget_block const* template_and_override_block);

	c_gui_data* get_data();
	int32 get_focused_item_index();
	int32 get_scroll_position();
	int32 get_selectable_item_count();
	bool set_focused_item_index(int32 focused_item_index, bool a2);
	void set_scroll_position(int32 scroll_position);
	void set_selectable_item_cap_count(int32 value);

//protected:
	s_runtime_list_widget_definition m_definition;
	c_string_id m_datasource_name;
	c_gui_bitmap_widget* m_more_items_preceeding_bitmap;
	c_gui_bitmap_widget* m_more_items_following_bitmap;
	int32 m_focused_item_index;
	e_list_scroll_direction m_current_scroll_direction;
	int32 m_scroll_position;
	int32 m_selectable_item_cap;
	c_gui_list_item_widget* m_submenu_item;
	bool m_is_submenu;
	bool m_is_submenu_needs_disposal;
};
static_assert(sizeof(c_gui_list_widget) == 0x158);
static_assert(sizeof(c_gui_list_widget) == sizeof(c_gui_widget) + 0x78);
static_assert(0x0E0 == OFFSETOF(c_gui_list_widget, m_definition));
static_assert(0x134 == OFFSETOF(c_gui_list_widget, m_datasource_name));
static_assert(0x138 == OFFSETOF(c_gui_list_widget, m_more_items_preceeding_bitmap));
static_assert(0x13C == OFFSETOF(c_gui_list_widget, m_more_items_following_bitmap));
static_assert(0x140 == OFFSETOF(c_gui_list_widget, m_focused_item_index));
static_assert(0x144 == OFFSETOF(c_gui_list_widget, m_current_scroll_direction));
static_assert(0x148 == OFFSETOF(c_gui_list_widget, m_scroll_position));
static_assert(0x14C == OFFSETOF(c_gui_list_widget, m_selectable_item_cap));
static_assert(0x150 == OFFSETOF(c_gui_list_widget, m_submenu_item));
static_assert(0x154 == OFFSETOF(c_gui_list_widget, m_is_submenu));
static_assert(0x155 == OFFSETOF(c_gui_list_widget, m_is_submenu_needs_disposal));

