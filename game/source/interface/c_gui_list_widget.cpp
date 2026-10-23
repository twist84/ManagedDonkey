#include "interface/c_gui_list_widget.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "memory/module.hpp"
#include "text/font_cache.hpp"

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

	ASSERT(submenu_widget != nullptr && submenu_widget->get_parent()->get_parent() == this);

	submenu_widget->get_parent()->remove_child_widget(submenu_widget);
	ui_track_delete<c_gui_list_widget>(submenu_widget);
}

void c_gui_list_widget::get_child_list_item_text_bounds(c_font_cache_base* font_cache, rectangle2d* max_bounds, bool* all_text_precached)
{
	//INVOKE_CLASS_MEMBER(0x00B14D30, c_gui_list_widget, get_child_list_item_text_bounds, font_cache, max_bounds, all_text_precached);

	ASSERT(all_text_precached != nullptr);

	rectangle2d bounds;
	set_rectangle2d(max_bounds, INT16_MAX, INT16_MAX, INT16_MIN, INT16_MIN);

	*all_text_precached = true;

	for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
	{
		e_gui_widget_type child_type = child->get_type();
		if (child_type == _gui_list_item)
		{
			for (c_gui_widget* list_item_child = child->get_children(); list_item_child != nullptr; list_item_child = list_item_child->get_next())
			{
				e_gui_widget_type list_item_child_type = list_item_child->get_type();
				if (list_item_child_type == _gui_text)
				{
					c_gui_text_widget* text_widget = static_cast<c_gui_text_widget*>(list_item_child);

					if (text_widget->get_text_bounds(font_cache, &bounds))
					{
						max_bounds->x0 = MIN(max_bounds->x0, bounds.x0);
						max_bounds->x1 = MAX(max_bounds->x1, bounds.x1);
						max_bounds->y0 = MIN(max_bounds->y0, bounds.y0);
						max_bounds->y1 = MAX(max_bounds->y1, bounds.y1);
					}
					else
					{
						*all_text_precached = false;
					}
				}
			}
		}
	}
}

void c_gui_list_widget::get_child_list_item_widget_bounds(real_rectangle2d* max_bounds)
{
	//INVOKE_CLASS_MEMBER(0x00B14E90, c_gui_list_widget, get_child_list_item_widget_bounds, max_bounds);

	csmemset(max_bounds, 0, sizeof(*max_bounds));

	for (c_gui_widget* child = get_children(); child != nullptr; child = child->get_next())
	{
		e_gui_widget_type child_type = child->get_type();
		if (child_type == _gui_list_item)
		{
			for (c_gui_widget* list_item_child = child->get_children(); list_item_child != nullptr; list_item_child = list_item_child->get_next())
			{
				real_rectangle2d rectangle2d;
				list_item_child->get_container_current_bounds(&rectangle2d);

				max_bounds->x0 = MIN(max_bounds->x0, rectangle2d.x0);
				max_bounds->x1 = MAX(max_bounds->x1, rectangle2d.x1);
				max_bounds->y0 = MIN(max_bounds->y0, rectangle2d.y0);
				max_bounds->y1 = MAX(max_bounds->y1, rectangle2d.y1);
			}
		}
	}
}

s_runtime_core_widget_definition* c_gui_list_widget::get_core_definition()
{
	//return INVOKE_CLASS_MEMBER(0x00B14FD0, c_gui_list_widget, get_core_definition);

	return &m_definition;
}

c_gui_data* c_gui_list_widget::get_data()
{
	//return INVOKE_CLASS_MEMBER(0x00B14FE0, c_gui_list_widget, get_data);

	c_gui_data* data;

	long datasource_index = NONE;
	c_gui_screen_widget* parent_screen = get_parent_screen();
	if (parent_screen == nullptr)
	{
		data = nullptr;
	}
	else
	{
		data = parent_screen->get_data(m_datasource_name.get_value(), &datasource_index);
	}

	if (data == nullptr)
	{
		event(_event_message, "ui: failed to find list datasource '%s'", m_datasource_name.get_string());
	}

	return data;
}

int32 c_gui_list_widget::get_datasource_index()
{
	//return INVOKE_CLASS_MEMBER(0x00B15020, c_gui_list_widget, get_datasource_index);

	int32 datasource_index = NONE;

	c_gui_screen_widget* parent_screen = get_parent_screen();
	if (parent_screen == nullptr || parent_screen->get_data(m_datasource_name.get_value(), &datasource_index) == nullptr)
	{
		datasource_index = NONE;
	}

	return datasource_index;
}

int32 c_gui_list_widget::get_element_handle_from_list_item_index(int32 list_item_index)
{
	//return INVOKE_CLASS_MEMBER(0x00B15060, c_gui_list_widget, get_element_handle_from_list_item_index, list_item_index);

	int32 element_handle = NONE;
	c_gui_data* datasource = get_data();

	list_item_index += m_scroll_position;

	if (datasource != nullptr && list_item_index < datasource->get_current_item_count())
	{
		element_handle = datasource->get_first_element_handle();
		for (int32 item_index = 0; item_index < list_item_index; item_index++)
		{
			element_handle = datasource->get_next_element_handle(element_handle);
		}
	}

	return element_handle;
}

int32 c_gui_list_widget::get_focused_element_handle()
{
	//return INVOKE_CLASS_MEMBER(0x00B150E0, c_gui_list_widget, get_focused_element_handle);

	c_gui_data* datasource = get_data();
	int32 element_handle = NONE;

	if (datasource != nullptr)
	{
		element_handle = datasource->get_first_element_handle();

		int32 item_count = TMIN(m_focused_item_index, datasource->get_current_item_count());
		for (int32 item_index = 0; item_index < item_count; item_index++)
		{
			element_handle = datasource->get_next_element_handle(element_handle);
		}
	}

	return element_handle;
}

int32 c_gui_list_widget::get_focused_item_index()
{
	//return INVOKE_CLASS_MEMBER(0x00B15160, c_gui_list_widget, get_focused_item_index);

	return m_focused_item_index;
}

int32 c_gui_list_widget::get_item_count(bool include_empty_items)
{
	//return INVOKE_CLASS_MEMBER(0x00B15170, c_gui_list_widget, get_item_count, include_empty_items);

	int32 count = 0;

	for (c_gui_list_item_widget* list_item_widget = static_cast<c_gui_list_item_widget*>(get_first_child_widget_by_type(_gui_list_item));
		list_item_widget != nullptr;
		list_item_widget = list_item_widget->get_next_list_item_widget(!include_empty_items))
	{
		if (include_empty_items || list_item_widget->get_element_handle() != NONE)
		{
			count++;
		}
	}

	return count;
}

int32 c_gui_list_widget::get_list_item_index_from_element_handle(int32 element_handle)
{
	//return INVOKE_CLASS_MEMBER(0x00B151C0, c_gui_list_widget, get_list_item_index_from_element_handle, element_handle);

	int32 list_item_index = NONE;

	c_gui_data* datasource = get_data();
	if (datasource != nullptr)
	{
		int32 current_element_handle = datasource->get_first_element_handle();
		int32 current_list_item_index = m_scroll_position;

		while (current_element_handle != NONE)
		{
			if (element_handle == current_element_handle)
			{
				list_item_index = current_list_item_index;
				break;
			}

			current_list_item_index++;
			current_element_handle = datasource->get_next_element_handle(current_element_handle);
		}
	}

	return list_item_index;
}

int32 c_gui_list_widget::get_scroll_position()
{
	//return INVOKE_CLASS_MEMBER(0x00B15250, c_gui_list_widget, get_scroll_position);

	return m_scroll_position;
}

int32 c_gui_list_widget::get_selectable_item_count()
{
	//return INVOKE_CLASS_MEMBER(0x00B15260, c_gui_list_widget, get_selectable_item_count);

	int32 result = 0;

	c_gui_data* datasource = get_data();
	if (datasource != nullptr)
	{
		if (m_selectable_item_cap == NONE)
		{
			result = datasource->get_current_item_count();
		}
		else
		{
			result = TMIN(datasource->get_current_item_count(), m_selectable_item_cap);
		}
	}

	return result;
}

//.text:00B152C0 ; 
//.text:00B15390 ; 

bool c_gui_list_widget::has_active_submenu() const
{
	return m_submenu_item != nullptr;
}

bool c_gui_list_widget::handle_grid_tab(c_gui_list_item_widget* current_item, e_event_type event_type)
{
	//return INVOKE_CLASS_MEMBER(0x00B15450, c_gui_list_widget, handle_grid_tab, current_item, event_type);

	bool handled = false;

	int32 max_item_count = get_item_count(true);
	int32 item_count = get_item_count(false);
	int32 rows = m_definition.rows;
	int32 columns = max_item_count / rows;

	if (item_count > 1)
	{
		if (max_item_count % rows == 0)
		{
			int32 current_row = current_item->get_list_item_index() / columns;
			int32 current_column = current_item->get_list_item_index() % columns;

			int32 current_item_count;
			if (get_data() != nullptr)
			{
				current_item_count = get_data()->get_current_item_count();
			}
			else
			{
				current_item_count = 0;
			}

			switch (event_type)
			{
			case _event_type_tab_up:
			{
				current_row--;
			}
			break;
			case _event_type_tab_left:
			{
				current_column--;
			}
			break;
			case _event_type_tab_down:
			{
				current_row++;
			}
			break;
			case _event_type_tab_right:
			{
				current_column++;
			}
			break;
			default:
				break;
			}

			if (list_wraps())
			{
				int32 actual_rows = item_count / columns + 1;
				int32 actual_columns = item_count >= columns ? columns : item_count;

				if (!VALID_INDEX(current_row, actual_rows) || !VALID_INDEX(current_column, actual_columns)
					|| !VALID_INDEX(current_row * columns + current_column, item_count))
				{
					switch (event_type)
					{
					case _event_type_tab_up:
					{
						current_row = actual_rows - 1;
						if (current_row * columns + current_column >= item_count)
						{
							current_row--;
						}
					}
					break;
					case _event_type_tab_left:
					{
						current_column = actual_columns - 1;
						if (current_row * columns + current_column >= item_count)
						{
							current_column = item_count % columns - 1;
						}
					}
					break;
					case _event_type_tab_down:
					{
						current_row = 0;
					}
					break;
					case _event_type_tab_right:
					{
						current_column = 0;
					}
					break;
					default:
						break;
					}
				}
			}

			int32 list_item_index = current_row * columns + m_scroll_position + current_column;
			if (event_type == _event_type_tab_down && list_item_index >= current_item_count)
			{
				if (list_item_index < ((current_item_count - 1) / columns + 1) * columns)
				{
					list_item_index = current_item_count - 1;
				}
			}

			if (current_item->get_list_item_index() + m_scroll_position != list_item_index
				&& VALID_INDEX(current_column, columns)
				&& get_data() != nullptr
				&& VALID_INDEX(list_item_index, current_item_count))
			{
				set_focused_item_index(list_item_index, true);

				handled = true;
			}
		}
		else
		{
			event(_event_error, "ui:c_gui_list_widget: the total number of list items widgets must be evenly divisible by the number of rows.");
		}
	}

	return handled;
}


bool c_gui_list_widget::handle_tab(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00B156B0, c_gui_list_widget, handle_tab, message);

	bool handled = false;

	c_gui_widget* widget = get_parent_screen()->get_focused_widget();

	if (!has_active_submenu() && get_enabled() && widget != nullptr && widget->get_type() == _gui_list_item && widget->get_parent_list() == this)
	{
		c_gui_list_item_widget* current_item = static_cast<c_gui_list_item_widget*>(widget);

		if (static_cast<uns32>(m_definition.rows) > 1)
		{
			if (scrolls_vertically())
			{
				e_controller_component component = message->get_component();
				e_event_type event_type = message->get_event_type();

				if (event_type == _event_type_button_press
					&& (component == _controller_component_button_left_trigger || component == _controller_component_button_right_trigger))
				{
					handle_tab_direction(current_item, component == _controller_component_button_left_trigger
						? _list_scroll_direction_page_previous
						: _list_scroll_direction_page_next);
				}
			}

			if (scrolls_horizontally())
			{
				event(_event_warning, "ui: 2d lists (lists with > 1 rows) cannot be marked as horizontal");
			}

			handled = handle_grid_tab(current_item, message->get_event_type());
		}
		else
		{
			e_controller_component component = message->get_component();
			e_event_type event_type = message->get_event_type();

			if (event_type == _event_type_button_press && scrolls_vertically()
				&& (component == _controller_component_button_left_trigger || component == _controller_component_button_right_trigger))
			{
				handled = handle_tab_direction(current_item, component == _controller_component_button_left_trigger
					? _list_scroll_direction_page_previous
					: _list_scroll_direction_page_next);
			}
			else if (scrolls_vertically())
			{
				switch (event_type)
				{
				case _event_type_tab_up:
				{
					handled = handle_tab_direction(current_item, _list_scroll_direction_backward);
				}
				break;
				case _event_type_tab_down:
				{
					handled = handle_tab_direction(current_item, _list_scroll_direction_forward);
				}
				break;
				default:
					break;
				}
			}
			else if (scrolls_horizontally())
			{
				switch (event_type)
				{
				case _event_type_tab_left:
				{
					handled = handle_tab_direction(current_item, _list_scroll_direction_backward);
				}
				break;
				case _event_type_tab_right:
				{
					handled = handle_tab_direction(current_item, _list_scroll_direction_forward);
				}
				break;
				default:
					break;
				}
			}
		}
	}

	if (!handled)
	{
		handled = c_gui_widget::handle_tab(message);
	}

	return handled;
}

bool c_gui_list_widget::handle_tab_direction(c_gui_list_item_widget* focused_item, e_list_scroll_direction scroll_direction)
{
	//return INVOKE_CLASS_MEMBER(0x00B157F0, c_gui_list_widget, handle_tab_direction, focused_item, scroll_direction);

	bool handled = false;

	c_gui_screen_widget* parent_screen = get_parent_screen();
	c_gui_data* datasource = get_data();

	int32 max_item_count = get_item_count(true);
	int32 rows = m_definition.rows;
	int32 columns = rows > 0 ? max_item_count / rows : 1;

	m_current_scroll_direction = scroll_direction;

	if (parent_screen != nullptr && datasource != nullptr)
	{
		int32 element_handle = NONE;

		if (scroll_direction == _list_scroll_direction_forward)
		{
			element_handle = datasource->get_next_element_handle(focused_item->get_element_handle());
			if (element_handle == NONE && list_wraps())
			{
				element_handle = datasource->get_first_element_handle();
			}
		}
		else if (scroll_direction == _list_scroll_direction_backward)
		{
			element_handle = datasource->get_previous_element_handle(focused_item->get_element_handle());
			if (element_handle == NONE && list_wraps())
			{
				while (datasource->get_next_element_handle(element_handle) != NONE)
				{
					element_handle = datasource->get_next_element_handle(element_handle);
				}
			}
		}
		else if (scroll_direction == _list_scroll_direction_page_next)
		{
			bool at_last_item = focused_item->get_next_list_item_widget(true) == nullptr;
			int32 current_list_position = 0;
			bool at_last_row = false;

			for (c_gui_list_item_widget* list_item_widget = static_cast<c_gui_list_item_widget*>(get_first_child_widget_by_type(_gui_list_item));
				list_item_widget != nullptr;
				list_item_widget = list_item_widget->get_next_list_item_widget(false))
			{
				if (list_item_widget == focused_item)
				{
					break;
				}

				current_list_position++;
			}

			if (rows > 1)
			{
				at_last_row = current_list_position >= (rows - 1) * columns;
			}

			int32 offset;
			if (at_last_item || at_last_row)
			{
				offset = get_item_count(true) - 1;
			}
			else
			{
				offset = TMIN(get_selectable_item_count(), get_item_count(false)) - current_list_position - 1;
			}

			if (columns > 1)
			{
				offset -= offset % columns;
			}

			element_handle = focused_item->get_element_handle();
			while (offset > 0)
			{
				if (datasource->get_next_element_handle(element_handle) == NONE)
				{
					break;
				}

				element_handle = datasource->get_next_element_handle(element_handle);
				offset--;
			}
		}
		else if (scroll_direction == _list_scroll_direction_page_previous)
		{
			bool at_first_item = focused_item->get_previous_list_item_widget(true) == nullptr;
			int32 current_list_position = 0;
			bool at_first_row = false;

			for (c_gui_list_item_widget* list_item_widget = static_cast<c_gui_list_item_widget*>(get_first_child_widget_by_type(_gui_list_item));
				list_item_widget != nullptr;
				list_item_widget = list_item_widget->get_next_list_item_widget(false))
			{
				if (list_item_widget == focused_item)
				{
					break;
				}

				current_list_position++;
			}

			if (rows > 1)
			{
				at_first_row = current_list_position < columns;
			}

			int32 offset = current_list_position;
			if (at_first_item || at_first_row)
			{
				offset = get_item_count(true) - 1;
			}

			if (columns > 1)
			{
				offset -= offset % columns;
			}

			element_handle = focused_item->get_element_handle();
			while (offset > 0)
			{
				if (datasource->get_previous_element_handle(element_handle) == NONE)
				{
					break;
				}

				element_handle = datasource->get_previous_element_handle(element_handle);
				offset--;
			}
		}

		if (element_handle != NONE && element_handle != focused_item->get_element_handle()
			&& set_focused_element_handle(element_handle, false))
		{
			parent_screen->transfer_focus_to_list(this, element_handle, true, true);

			handled = true;
		}
	}

	return handled;
}

bool c_gui_list_widget::handle_tab_to_list_item(c_gui_list_item_widget* old_focused_item, c_gui_list_item_widget* new_focused_item)
{
	//return INVOKE_CLASS_MEMBER(0x00B15B70, c_gui_list_widget, handle_tab_to_list_item, old_focused_item, new_focused_item);
	
	bool handled = true;

	m_focused_item_index = new_focused_item->get_element_index();
	c_gui_screen_widget* parent_screen = get_parent_screen();
	parent_screen->transfer_focus(new_focused_item);

	return handled;
}

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
	//return INVOKE_CLASS_MEMBER(0x00B15EB0, c_gui_list_widget, list_has_more_elements_following);

	bool more = false;
	c_gui_list_item_widget* last_item = nullptr;

	c_gui_data* datasource = get_data();
	for (c_gui_list_item_widget* list_item = static_cast<c_gui_list_item_widget*>(get_first_child_widget_by_type(_gui_list_item));
		list_item != nullptr;
		list_item = list_item->get_next_list_item_widget(true))
	{
		last_item = list_item;
	}

	if (last_item != nullptr && datasource != nullptr)
	{
		more = datasource->get_next_element_handle(last_item->get_element_handle()) != NONE;
	}

	return more;
}

bool c_gui_list_widget::list_has_more_elements_preceeding()
{
	//return INVOKE_CLASS_MEMBER(0x00B15F40, c_gui_list_widget, list_has_more_elements_preceeding);

	c_gui_list_item_widget* first_item = static_cast<c_gui_list_item_widget*>(get_first_child_widget_by_type(_gui_list_item));
	c_gui_data* datasource = get_data();

	int32 element_handle;
	if (first_item != nullptr)
	{
		element_handle = first_item->get_element_handle();
	}
	else
	{
		element_handle = NONE;
	}

	bool more = datasource != nullptr && datasource->get_previous_element_handle(element_handle) != NONE;
	return more;
}

bool c_gui_list_widget::list_selection_visible_without_focus() const
{
	return TEST_BIT(m_definition.flags, _list_widget_definition_flag_selection_visible_without_focus_bit);
}

bool c_gui_list_widget::list_wraps() const
{
	//return INVOKE_CLASS_MEMBER(0x00B15FB0, c_gui_list_widget, list_wraps);

	return TEST_BIT(m_definition.flags, _list_widget_definition_flag_list_wraps_bit);
}

void c_gui_list_widget::mark_as_submenu(bool is_submenu)
{
	//INVOKE_CLASS_MEMBER(0x00B15FC0, c_gui_list_widget, mark_as_submenu, is_submenu);

	m_is_submenu = is_submenu;
}

void c_gui_list_widget::offset_horizontal_list_item_indicators()
{
	INVOKE_CLASS_MEMBER(0x00B15FD0, c_gui_list_widget, offset_horizontal_list_item_indicators);

	//if (m_more_items_following_bitmap != nullptr || m_more_items_preceeding_bitmap != nullptr)
	//{
	//	if (scrolls_horizontally())
	//	{
	//		rectangle2d max_bounds = {};
	//		bool all_text_precached = false;
	//		c_font_cache_mt_safe font_cache;
	//		get_child_list_item_text_bounds(&font_cache, &max_bounds, &all_text_precached);
	//
	//		if (m_more_items_preceeding_bitmap != nullptr)
	//		{
	//			s_animation_transform transform = *m_more_items_preceeding_bitmap->get_animated_state();
	//
	//			m_more_items_preceeding_bitmap->set_full_animation_state(&transform, false);
	//			m_more_items_preceeding_bitmap->set_visible(all_text_precached);
	//		}
	//		if (m_more_items_following_bitmap != nullptr)
	//		{
	//			s_animation_transform transform = *m_more_items_following_bitmap->get_animated_state();
	//			real_vector2d dimensions_scale = interface_get_authoring_to_target_dimensions_scaling();
	//
	//			{
	//				int16 x0 = max_bounds.x0;
	//				int16 x1 = max_bounds.x1;
	//				transform.position.x += static_cast<float>(x1 - x0) / dimensions_scale.i;
	//			}
	//
	//			m_more_items_following_bitmap->set_full_animation_state(&transform, false);
	//			m_more_items_following_bitmap->set_visible(all_text_precached);
	//		}
	//	}
	//	else
	//	{
	//		if (m_more_items_preceeding_bitmap != nullptr)
	//		{
	//			m_more_items_preceeding_bitmap->set_visible(true);
	//		}
	//		if (m_more_items_following_bitmap != nullptr)
	//		{
	//			m_more_items_following_bitmap->set_visible(true);
	//		}
	//	}
	//}
}

void c_gui_list_widget::post_initialize()
{
	//INVOKE_CLASS_MEMBER(0x00B16130, c_gui_list_widget, post_initialize);

	c_gui_widget::post_initialize();
}

bool c_gui_list_widget::scrolls_horizontally() const
{
	//return INVOKE(0x00B16140, c_gui_list_widget, scrolls_horizontally);

	return TEST_BIT(m_definition.flags, _list_widget_definition_flag_horizontal_list_bit);
}

bool c_gui_list_widget::scrolls_vertically()
{
	//return INVOKE(0x00B16150, c_gui_list_widget, scrolls_vertically);

	return !scrolls_horizontally();
}

void c_gui_list_widget::set_datasource_name(string_id name)
{
	//INVOKE_CLASS_MEMBER(0x00B16160, c_gui_list_widget, set_datasource_name, name);

	m_datasource_name = name;
}

bool c_gui_list_widget::set_focused_datasource_element_from_value(int32 value_name, int32 value, bool play_animations_on_focus_change)
{
	//return INVOKE_CLASS_MEMBER(0x00B16170, c_gui_list_widget, set_focused_datasource_element_from_value, value_name, value, play_animations_on_focus_change);

	c_gui_data* datasource = get_data();
	bool success = false;

	if (datasource != nullptr)
	{
		for (int32 element_handle = datasource->get_first_element_handle(); element_handle != NONE; element_handle = datasource->get_next_element_handle(element_handle))
		{
			int32 integer_value;
			if (datasource->get_integer_value(element_handle, value_name, &integer_value))
			{
				if (integer_value == value)
				{
					set_focused_element_handle(element_handle, play_animations_on_focus_change);
					success = true;
					break;
				}
			}
		}
	}

	return success;
}

bool c_gui_list_widget::set_focused_element_handle(int32 element_handle, bool play_animations_on_focus_change)
{
	//return INVOKE_CLASS_MEMBER(0x00B16230, c_gui_list_widget, set_focused_element_handle, element_handle, play_animations_on_focus_change);

	bool changed = false;
	c_gui_data* datasource = get_data();

	if (datasource != nullptr)
	{
		int32 find_element_handle = datasource->get_first_element_handle();
		for (int32 item_index = 0; item_index < datasource->get_current_item_count(); item_index++)
		{
			if (element_handle == find_element_handle)
			{
				changed = set_focused_item_index(item_index, play_animations_on_focus_change);
				break;
			}

			find_element_handle = datasource->get_next_element_handle(find_element_handle);
		}
	}

	return changed;
}

bool c_gui_list_widget::set_focused_item_index(int32 focused_item_index, bool play_animations_on_focus_change)
{
	return INVOKE_CLASS_MEMBER(0x00B162E0, c_gui_list_widget, set_focused_item_index, focused_item_index, play_animations_on_focus_change);
}

void c_gui_list_widget::set_scroll_position(int32 scroll_position)
{
	//INVOKE_CLASS_MEMBER(0x00B16440, c_gui_list_widget, set_scroll_position, scroll_position);

	c_gui_data* datasource = get_data();
	if (datasource != nullptr)
	{
		int32 item_count = datasource->get_current_item_count();
		if ((item_count == 0 && scroll_position == 0) || scroll_position < item_count)
		{
			m_scroll_position = scroll_position;
		}
	}
}

c_gui_list_item_widget* c_gui_list_widget::try_and_get_child_list_item_widget_by_element_handle(int32 element_handle)
{
	//return INVOKE_CLASS_MEMBER(0x00B16490, c_gui_list_widget, try_and_get_child_list_item_widget_by_element_handle, element_handle);

	c_gui_list_item_widget* item = nullptr;
	if (element_handle != NONE)
	{
		for (c_gui_list_item_widget* test_item = static_cast<c_gui_list_item_widget*>(get_first_child_widget_by_type(_gui_list_item)); test_item != nullptr; test_item = test_item->get_next_list_item_widget(true))
		{
			int32 test_element_handle = test_item->get_element_handle();
			if (test_element_handle == element_handle)
			{
				item = test_item;
				break;
			}
		}
	}
	return item;
}

c_gui_list_item_widget* c_gui_list_widget::try_and_get_focused_child_list_item_widget()
{
	//return INVOKE_CLASS_MEMBER(0x00B164F0, c_gui_list_widget, try_and_get_focused_child_list_item_widget);

	int32 element_handle = get_focused_element_handle();
	c_gui_list_item_widget* item = try_and_get_child_list_item_widget_by_element_handle(element_handle);
	return item;
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
	//INVOKE_CLASS_MEMBER(0x00B165D0, c_gui_list_widget, update_indicator_bitmap_widget_animation_states);

	bool more_preceeding_items = list_has_more_elements_preceeding();
	bool more_following_items = list_has_more_elements_following();

	if (m_more_items_preceeding_bitmap != nullptr)
	{
		if (m_current_scroll_direction == _list_scroll_direction_backward || m_current_scroll_direction == _list_scroll_direction_page_previous)
		{
			e_animation_state animation_state = more_preceeding_items ? _indicator_activated_additional_items : _indicator_activated_no_additional_items;
			m_more_items_preceeding_bitmap->start_animation(animation_state, false);
		}
	}

	if (m_more_items_following_bitmap != nullptr)
	{
		if (m_current_scroll_direction == _list_scroll_direction_forward || m_current_scroll_direction == _list_scroll_direction_page_next)
		{
			e_animation_state animation_state = more_following_items ? _indicator_activated_additional_items : _indicator_activated_no_additional_items;
			m_more_items_following_bitmap->start_animation(animation_state, false);
		}
	}
}

void c_gui_list_widget::update_render_state(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00B16650, c_gui_list_widget, update_render_state, current_milliseconds);

	c_gui_widget::update_render_state(current_milliseconds);
	c_gui_list_widget::offset_horizontal_list_item_indicators();
}

