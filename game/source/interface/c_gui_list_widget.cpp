#include "interface/c_gui_list_widget.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B14B90, c_gui_list_widget, close_active_submenu_);

void __thiscall c_gui_list_widget::close_active_submenu_(c_gui_list_widget* submenu_widget)
{
	c_gui_list_widget::close_active_submenu(submenu_widget);
}

template<>
void ui_track_delete<c_gui_list_widget>(const c_gui_list_widget* object)
{
	ASSERT(object != nullptr);

	object->~c_gui_list_widget();
	user_interface_free(object);
}

c_gui_list_widget::c_gui_list_widget() :
	c_gui_widget(_gui_list),
	m_definition(),
	m_datasource_name(NONE),
	m_more_items_preceeding_bitmap(nullptr),
	m_more_items_following_bitmap(nullptr),
	m_focused_item_index(0),
	m_current_scroll_direction(_list_scroll_direction_none),
	m_scroll_position(0),
	m_selectable_item_cap(NONE),
	m_submenu_item(nullptr),
	m_is_submenu(false),
	m_is_submenu_needs_disposal(false)
{
	//DECLFUNC(0x00B14890, void, __thiscall, c_gui_list_widget*)(this);
}

s_list_widget_block::s_list_widget_block()
{
	DECLFUNC(0x00B14910, void, __thiscall, s_list_widget_block*)(this);
}

s_list_widget_definition::s_list_widget_definition()
{
	DECLFUNC(0x00B14970, void, __thiscall, s_list_widget_definition*)(this);
}

s_runtime_list_widget_definition::s_runtime_list_widget_definition()
{
	DECLFUNC(0x00B149E0, void, __thiscall, s_runtime_list_widget_definition*)(this);
}

//.text:00B14A70 ; 

//.text:00B14A80 ; public: virtual void* c_gui_list_widget::`scalar deleting destructor'(unsigned int)

void c_gui_list_widget::add_definition_fields(const s_list_widget_definition* definition, bool was_templated)
{
	INVOKE_CLASS_MEMBER(0x00B14AB0, c_gui_list_widget, add_definition_fields, definition, was_templated);
}

void c_gui_list_widget::close_active_submenu(c_gui_list_widget* submenu_widget)
{
	//INVOKE_CLASS_MEMBER(0x00B14B90, c_gui_list_widget, close_active_submenu, submenu_widget);

	ASSERT(submenu_widget != nullptr);
	ASSERT(m_submenu_item != nullptr);

	c_gui_screen_widget* parent_screen = get_parent_screen();
	ASSERT(parent_screen != nullptr);

	parent_screen->transfer_focus_without_animations(m_submenu_item, false, true);
	m_submenu_item = nullptr;
	c_gui_list_widget::dispose_submenu(submenu_widget);
	c_gui_widget::animate_recursively(m_last_animated_milliseconds);
}

void c_gui_list_widget::create_and_add_additional_items_indicators_bitmaps()
{
	//INVOKE_CLASS_MEMBER(0x00B14C10, c_gui_list_widget, create_and_add_additional_items_indicators_bitmaps);

	s_bitmap_widget_block bitmap_widget_block;
	csmemset(&bitmap_widget_block, 0, sizeof(bitmap_widget_block));
	bitmap_widget_block.widget_template_reference.group_tag = GUI_BITMAP_WIDGET_DEFINITION_TAG;
	bitmap_widget_block.widget_template_reference.index = NONE;

	if (m_definition.previous_indicator_bitmap_reference_index != NONE)
	{
		s_bitmap_widget_definition* bitmap_widget_definition = TAG_GET(GUI_BITMAP_WIDGET_DEFINITION_TAG, s_bitmap_widget_definition, m_definition.previous_indicator_bitmap_reference_index);
		bitmap_widget_block.override_definition = *bitmap_widget_definition;
		m_more_items_preceeding_bitmap = c_gui_widget::create_and_add_child_bitmap_widget(&bitmap_widget_block);
	}

	if (m_definition.next_indicator_bitmap_reference_index != NONE)
	{
		s_bitmap_widget_definition* bitmap_widget_definition = TAG_GET(GUI_BITMAP_WIDGET_DEFINITION_TAG, s_bitmap_widget_definition, m_definition.next_indicator_bitmap_reference_index);
		bitmap_widget_block.override_definition = *bitmap_widget_definition;
		m_more_items_following_bitmap = c_gui_widget::create_and_add_child_bitmap_widget(&bitmap_widget_block);
	}

	if (m_more_items_preceeding_bitmap)
	{
		m_more_items_preceeding_bitmap->set_visible(false);
	}

	if (m_more_items_following_bitmap)
	{
		m_more_items_following_bitmap->set_visible(false);
	}
}

void c_gui_list_widget::dispose_submenu(c_gui_list_widget* submenu_widget)
{
	//INVOKE_CLASS_MEMBER(0x00B14CE0, c_gui_list_widget, dispose_submenu, submenu_widget);

	submenu_widget->get_parent()->remove_child_widget(submenu_widget);
	ui_track_delete<c_gui_list_widget>(submenu_widget);
}

//.text:00B14D30 ; public: void c_gui_list_widget::get_child_list_item_text_bounds(c_font_cache_base*, rectangle2d*, bool*)
//.text:00B14E90 ; public: void c_gui_list_widget::get_child_list_item_widget_bounds(real_rectangle2d*)

s_runtime_core_widget_definition* c_gui_list_widget::get_core_definition()
{
	//return INVOKE_CLASS_MEMBER(0x00B14FD0, c_gui_list_widget, get_core_definition);

	return &m_definition;
}

c_gui_data* c_gui_list_widget::get_data()
{
	//return INVOKE_CLASS_MEMBER(0x00B14FE0, c_gui_list_widget, get_data);

	c_gui_screen_widget* parent_screen = get_parent_screen();
	if (!parent_screen)
	{
		event(_event_message, "ui: failed to find list datasource '%s'", m_datasource_name.get_string());
		return nullptr;
	}

	int32 datasource_index = NONE;
	c_gui_data* datasource = parent_screen->get_data(m_datasource_name.get_value(), &datasource_index);
	if (!datasource)
	{
		return nullptr;
	}

	return datasource;
}

int32 c_gui_list_widget::get_datasource_index()
{
	//return INVOKE_CLASS_MEMBER(0x00B15020, c_gui_list_widget, get_datasource_index);

	int32 datasource_index = NONE;
	c_gui_screen_widget* parent_screen = get_parent_screen();
	if (!parent_screen || !parent_screen->get_data(m_datasource_name.get_value(), &datasource_index))
	{
		return NONE;
	}

	return datasource_index;
}

int32 c_gui_list_widget::get_element_handle_from_list_item_index(int32 list_item_index)
{
	return INVOKE_CLASS_MEMBER(0x00B15060, c_gui_list_widget, get_element_handle_from_list_item_index, list_item_index);
}

int32 c_gui_list_widget::get_focused_element_handle()
{
	return INVOKE_CLASS_MEMBER(0x00B150E0, c_gui_list_widget, get_focused_element_handle);
}

int32 c_gui_list_widget::get_focused_item_index()
{
	//return INVOKE_CLASS_MEMBER(0x00B15160, c_gui_list_widget, get_focused_item_index);

	return m_focused_item_index;
}

int32 c_gui_list_widget::get_item_count(bool include_empty_items)
{
	return INVOKE_CLASS_MEMBER(0x00B15170, c_gui_list_widget, get_item_count, include_empty_items);
}

int32 c_gui_list_widget::get_list_item_index_from_element_handle(int32 element_handle)
{
	return INVOKE_CLASS_MEMBER(0x00B151C0, c_gui_list_widget, get_list_item_index_from_element_handle, element_handle);
}

int32 c_gui_list_widget::get_scroll_position()
{
	//return INVOKE_CLASS_MEMBER(0x00B15250, c_gui_list_widget, get_scroll_position);

	return m_scroll_position;
}

int32 c_gui_list_widget::get_selectable_item_count()
{
	//return INVOKE_CLASS_MEMBER(0x00B15260, c_gui_list_widget, get_selectable_item_count);

	// Halo Online, probably an ODST change
	//int32 datasource_index = NONE;
	//c_gui_screen_widget* parent_screen = c_gui_widget::get_parent_screen();
	//if (!parent_screen)
	//{
	//	return 0;
	//}
	//
	//c_gui_data* data = parent_screen->get_data(m_datasource_name.get_value(), &datasource_index);

	c_gui_data* data = c_gui_list_widget::get_data();
	if (!data)
	{
		return 0;
	}

	if (m_selectable_item_cap == NONE)
	{
		return data->get_current_item_count();
	}

	int32 current_item_count = data->get_current_item_count();
	if (current_item_count > m_selectable_item_cap)
	{
		return m_selectable_item_cap;
	}

	return current_item_count;
}

//.text:00B152C0 ; 
//.text:00B15390 ; 

bool c_gui_list_widget::has_active_submenu() const
{
	return m_submenu_item != nullptr;
}

//.text:00B15450 ; private: bool c_gui_list_widget::handle_grid_tab(c_gui_list_item_widget*, e_event_type)

bool c_gui_list_widget::handle_tab(const c_controller_input_message* message)
{
	return INVOKE_CLASS_MEMBER(0x00B156B0, c_gui_list_widget, handle_tab, message);
}

//.text:00B157F0 ; private: bool c_gui_list_widget::handle_tab_direction(c_gui_list_item_widget*, e_list_scroll_direction)
//.text:00B15B70 ; private: bool c_gui_list_widget::handle_tab_to_list_item(c_gui_list_item_widget*, c_gui_list_item_widget*)

void c_gui_list_widget::initialize(const s_list_widget_block* template_and_override_block)
{
	//INVOKE_CLASS_MEMBER(0x00B15BA0, c_gui_list_widget, initialize, template_and_override_block);

	if (template_and_override_block)
	{
		real_rectangle2d positioning_bounds{};
		m_parent.get_value()->get_authored_bounds(&positioning_bounds);

		bool was_templated = false;
		if (template_and_override_block->widget_template_reference.index != NONE)
		{
			s_list_widget_definition* list_widget_definition = TAG_GET(GUI_LIST_WIDGET_DEFINITION_TAG, s_list_widget_definition, template_and_override_block->widget_template_reference.index);
			c_gui_list_widget::add_definition_fields(list_widget_definition, false);
			was_templated = true;
		}
		c_gui_list_widget::add_definition_fields(&template_and_override_block->override_definition, was_templated);
	}

	m_name = m_definition.widget_identifier;
	c_gui_widget::create_and_add_child_list_item_widgets(&m_definition.list_items, m_definition.gui_skin_reference_index);
	c_gui_list_widget::create_and_add_additional_items_indicators_bitmaps();
	c_gui_widget::initialize();
}

bool c_gui_list_widget::invoke_submenu(int32 submenu_name, int32 custom_datasource_name, bool reset_datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00B15C20, c_gui_list_widget, invoke_submenu, submenu_name, custom_datasource_name, reset_datasource);
	return DECLFUNC(0x00B15C20, bool, __thiscall, c_gui_list_widget*, int32, int32, bool)(this, submenu_name, custom_datasource_name, reset_datasource);
}

bool c_gui_list_widget::invoke_submenu(const s_list_widget_block* submenu_definition, int32 custom_datasource_name, bool reset_datasource)
{
	//return INVOKE_CLASS_MEMBER(0x00B15CD0, c_gui_list_widget, invoke_submenu, submenu_definition, custom_datasource_name, reset_datasource);
	return DECLFUNC(0x00B15CD0, bool, __thiscall, c_gui_list_widget*, const s_list_widget_block*, int32, bool)(this, submenu_definition, custom_datasource_name, reset_datasource);
}

bool c_gui_list_widget::is_submenu() const
{
	return m_is_submenu;
}

bool c_gui_list_widget::is_submenu_that_needs_disposal() const
{
	//return INVOKE_CLASS_MEMBER(0x00B15EA0, c_gui_list_widget, is_submenu_that_needs_disposal);

	return m_is_submenu_needs_disposal;
}

bool c_gui_list_widget::list_has_more_elements_following()
{
	return INVOKE_CLASS_MEMBER(0x00B15EB0, c_gui_list_widget, list_has_more_elements_following);
}

bool c_gui_list_widget::list_has_more_elements_preceeding()
{
	return INVOKE_CLASS_MEMBER(0x00B15F40, c_gui_list_widget, list_has_more_elements_preceeding);
}

bool c_gui_list_widget::list_selection_visible_without_focus() const
{
	bool result = TEST_BIT(m_definition.flags, _list_widget_definition_flag_selection_visible_without_focus_bit);
	return result;
}

bool c_gui_list_widget::list_wraps() const
{
	return INVOKE_CLASS_MEMBER(0x00B15FB0, c_gui_list_widget, list_wraps);
}

void c_gui_list_widget::mark_as_submenu(bool is_submenu)
{
	//INVOKE_CLASS_MEMBER(0x00B15FC0, c_gui_list_widget, mark_as_submenu, is_submenu);

	m_is_submenu = is_submenu;
}

void c_gui_list_widget::offset_horizontal_list_item_indicators()
{
	INVOKE_CLASS_MEMBER(0x00B15FD0, c_gui_list_widget, offset_horizontal_list_item_indicators);
}

void c_gui_list_widget::post_initialize()
{
	//INVOKE_CLASS_MEMBER(0x00B16130, c_gui_list_widget, post_initialize);

	c_gui_widget::post_initialize();
}

//.text:00B16140 ; public: bool c_gui_list_widget::scrolls_horizontally() const
//.text:00B16150 ; public: bool c_gui_list_widget::scrolls_vertically()
//.text:00B16160 ; private: void c_gui_list_widget::set_datasource_name(int32)

bool c_gui_list_widget::set_focused_datasource_element_from_value(int32 value_name, int32 value, bool play_animations_on_focus_change)
{
	return INVOKE_CLASS_MEMBER(0x00B16170, c_gui_list_widget, set_focused_datasource_element_from_value, value_name, value, play_animations_on_focus_change);
}

bool c_gui_list_widget::set_focused_element_handle(int32 element_handle, bool play_animations_on_focus_change)
{
	return INVOKE_CLASS_MEMBER(0x00B16230, c_gui_list_widget, set_focused_element_handle, element_handle, play_animations_on_focus_change);
}

bool c_gui_list_widget::set_focused_item_index(int32 focused_item_index, bool play_animations_on_focus_change)
{
	return INVOKE_CLASS_MEMBER(0x00B162E0, c_gui_list_widget, set_focused_item_index, focused_item_index, play_animations_on_focus_change);
}

void c_gui_list_widget::set_scroll_position(int32 scroll_position)
{
	INVOKE_CLASS_MEMBER(0x00B16440, c_gui_list_widget, set_scroll_position, scroll_position);
}

c_gui_list_item_widget* c_gui_list_widget::try_and_get_child_list_item_widget_by_element_handle(int32 element_handle)
{
	return INVOKE_CLASS_MEMBER(0x00B16490, c_gui_list_widget, try_and_get_child_list_item_widget_by_element_handle, element_handle);
}

c_gui_list_item_widget* c_gui_list_widget::try_and_get_focused_child_list_item_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00B164F0, c_gui_list_widget, try_and_get_focused_child_list_item_widget);

	int32 focused_element_handle = c_gui_list_widget::get_focused_element_handle();
	return c_gui_list_widget::try_and_get_child_list_item_widget_by_element_handle(focused_element_handle);
}

void c_gui_list_widget::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00B16510, c_gui_list_widget, update, current_milliseconds);

	c_gui_widget::update(current_milliseconds);

	if (m_focused_item_index >= c_gui_list_widget::get_selectable_item_count())
	{
		if (c_gui_screen_widget* parent_screen = c_gui_widget::get_parent_screen())
		{
			int32 selectable_item_count = c_gui_list_widget::get_selectable_item_count();
			int32 element_handle = c_gui_list_widget::get_element_handle_from_list_item_index(selectable_item_count - 1);
			parent_screen->transfer_focus_to_list(this, element_handle, false, false);
		}
	}

	c_gui_list_widget::update_indicator_bitmap_widget_animation_states();

	m_current_scroll_direction = _list_scroll_direction_none;

	if (c_gui_list_widget::is_submenu_that_needs_disposal())
	{
		c_gui_list_widget* parent_list = c_gui_widget::get_parent_list();
		ASSERT(parent_list != nullptr);
		parent_list->close_active_submenu(this);
	}
}

void c_gui_list_widget::update_indicator_bitmap_widget_animation_states()
{
	INVOKE_CLASS_MEMBER(0x00B165D0, c_gui_list_widget, update_indicator_bitmap_widget_animation_states);
}

void c_gui_list_widget::update_render_state(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00B16650, c_gui_list_widget, update_render_state, current_milliseconds);

	c_gui_widget::update_render_state(current_milliseconds);
	c_gui_list_widget::offset_horizontal_list_item_indicators();
}

