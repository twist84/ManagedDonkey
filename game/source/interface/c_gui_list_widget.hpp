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
	s_list_widget_definition();

	c_string_id datasource_name;
	s_tag_reference skin;
	int32 rows;
	s_tag_block items;
	s_tag_reference prev_indicator_bitmap;
	s_tag_reference next_indicator_bitmap;
};
COMPILE_ASSERT(sizeof(s_list_widget_definition) == sizeof(s_core_widget_definition) + 0x44);

struct s_runtime_list_widget_definition :
	s_runtime_core_widget_definition
{
	s_runtime_list_widget_definition();

	c_string_id datasource_name;
	int32 gui_skin_reference_index;
	int32 rows;
	s_tag_block list_items;
	int32 previous_indicator_bitmap_reference_index;
	int32 next_indicator_bitmap_reference_index;
};
COMPILE_ASSERT(sizeof(s_runtime_list_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x20);

struct s_list_widget_block
{
	s_list_widget_block();

	s_tag_reference widget_template_reference;
	s_list_widget_definition override_definition;
};
COMPILE_ASSERT(sizeof(s_list_widget_block) == 0x80);

class c_gui_data;
class c_gui_list_widget :
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
	void __thiscall close_active_submenu_(c_gui_list_widget* submenu_widget);

public:
	virtual ~c_gui_list_widget() = default;
	virtual void post_initialize() override;
	virtual int32 get_datasource_index() override;
	virtual s_runtime_core_widget_definition* get_core_definition() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual void update_render_state(uns32 current_milliseconds) override;
	virtual bool handle_tab(const c_controller_input_message* message) override;
	virtual void initialize(const s_list_widget_block* template_and_override_block);

public:
	c_gui_list_widget();
	void add_definition_fields(const s_list_widget_definition* definition, bool was_templated);
	void close_active_submenu(c_gui_list_widget* submenu_widget);
	void create_and_add_additional_items_indicators_bitmaps();
	void dispose_submenu(c_gui_list_widget* submenu_widget);
	c_gui_data* get_data();
	int32 get_element_handle_from_list_item_index(int32 list_item_index);
	int32 get_focused_element_handle();
	int32 get_focused_item_index();
	int32 get_item_count(bool include_empty_items);
	int32 get_list_item_index_from_element_handle(int32 element_handle);
	int32 get_scroll_position();
	int32 get_selectable_item_count();
	bool invoke_submenu(int32 submenu_name, int32 custom_datasource_name, bool reset_datasource);
	bool invoke_submenu(const s_list_widget_block* submenu_definition, int32 custom_datasource_name, bool reset_datasource);
	bool is_submenu_that_needs_disposal() const;
	bool list_has_more_elements_following();
	bool list_has_more_elements_preceeding();
	bool list_wraps() const;
	void mark_as_submenu(bool is_submenu);

private:
	void offset_horizontal_list_item_indicators();

public:
	void mark_as_submenu_that_needs_disposal(bool dispose);
	bool set_focused_datasource_element_from_value(int32 value_name, int32 value, bool play_animations_on_focus_change);
	bool set_focused_element_handle(int32 element_handle, bool play_animations_on_focus_change);
	bool set_focused_item_index(int32 focused_item_index, bool play_animations_on_focus_change);
	void set_scroll_position(int32 scroll_position);
	c_gui_list_item_widget* try_and_get_child_list_item_widget_by_element_handle(int32 element_handle);
	c_gui_list_item_widget* try_and_get_focused_child_list_item_widget();
	void set_selectable_item_cap_count(int32 value);

private: 
	void update_indicator_bitmap_widget_animation_states();

public:
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
COMPILE_ASSERT(sizeof(c_gui_list_widget) == 0x158);
COMPILE_ASSERT(sizeof(c_gui_list_widget) == sizeof(c_gui_widget) + 0x78);
COMPILE_ASSERT(0x0E0 == OFFSETOF(c_gui_list_widget, m_definition));
COMPILE_ASSERT(0x134 == OFFSETOF(c_gui_list_widget, m_datasource_name));
COMPILE_ASSERT(0x138 == OFFSETOF(c_gui_list_widget, m_more_items_preceeding_bitmap));
COMPILE_ASSERT(0x13C == OFFSETOF(c_gui_list_widget, m_more_items_following_bitmap));
COMPILE_ASSERT(0x140 == OFFSETOF(c_gui_list_widget, m_focused_item_index));
COMPILE_ASSERT(0x144 == OFFSETOF(c_gui_list_widget, m_current_scroll_direction));
COMPILE_ASSERT(0x148 == OFFSETOF(c_gui_list_widget, m_scroll_position));
COMPILE_ASSERT(0x14C == OFFSETOF(c_gui_list_widget, m_selectable_item_cap));
COMPILE_ASSERT(0x150 == OFFSETOF(c_gui_list_widget, m_submenu_item));
COMPILE_ASSERT(0x154 == OFFSETOF(c_gui_list_widget, m_is_submenu));
COMPILE_ASSERT(0x155 == OFFSETOF(c_gui_list_widget, m_is_submenu_needs_disposal));

