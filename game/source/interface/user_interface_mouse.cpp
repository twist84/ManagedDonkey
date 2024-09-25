#include "interface/user_interface_mouse.hpp"

#include "camera/director.hpp"
#include "input/input_windows.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/interface_constants.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_controller.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "shell/shell_windows.hpp"

#define WIDGET_GET_FIRST_CHILD(WIDGET) (WIDGET)->m_children.__unknown0
#define WIDGET_GET_NEXT_CHILD(WIDGET) (WIDGET)->m_next.__unknown0

s_user_interface_mouse_globals user_interface_mouse_globals{};

void user_interface_mouse_update()
{
	if (g_windows_params.game_window_handle != GetForegroundWindow())
		return;

	if (director_get(_output_user_index0)->get_camera()->get_type() == _camera_mode_flying)
		return;

	user_interface_mouse_update_internal();

	c_gui_screen_widget* topmost_screen = window_manager_get()->get_topmost_screen(_window_index4);
	if (!topmost_screen)
		topmost_screen = window_manager_get()->get_topmost_screen(_window_index0);
	//if (!topmost_screen)
	//	topmost_screen = window_manager_get()->get_topmost_screen(_window_index1);
	//if (!topmost_screen)
	//	topmost_screen = window_manager_get()->get_topmost_screen(_window_index2);
	//if (!topmost_screen)
	//	topmost_screen = window_manager_get()->get_topmost_screen(_window_index3);

	if (topmost_screen && topmost_screen->get_focused_widget())
	{
		if (!user_interface_mouse_handle_screen_widget(topmost_screen))
			user_interface_mouse_globals.last_focused_widget = NULL;
	}
}

void widget_bounds_from_window_bounds(real_rectangle2d const* window_bounds, real_rectangle2d* widget_bounds)
{
	vector2d aspect_ratio_scaling = interface_get_aspect_ratio_scaling();

	real bounds_x = window_bounds->x1 - window_bounds->x0;
	real bounds_y = window_bounds->y1 - window_bounds->y0;
	real adjusted_x = real(bounds_x / 2) + window_bounds->x0;
	real adjusted_y = real(bounds_y / 2) + window_bounds->y0;
	real normalized_x = real(bounds_x / real(c_rasterizer::render_globals.resolution_width));
	real normalized_y = real(bounds_y / real(c_rasterizer::render_globals.resolution_height));

	widget_bounds->x0 = ((widget_bounds->x0 * normalized_x) * aspect_ratio_scaling.i) + adjusted_x;
	widget_bounds->x1 = ((widget_bounds->x1 * normalized_x) * aspect_ratio_scaling.i) + adjusted_x;
	widget_bounds->y0 = ((widget_bounds->y0 * normalized_y) * aspect_ratio_scaling.j) + adjusted_y;
	widget_bounds->y1 = ((widget_bounds->y1 * normalized_y) * aspect_ratio_scaling.j) + adjusted_y;
}

void user_interface_mouse_update_tracking()
{
	static bool cursor_shown = false;

	bool screen_active = g_window_manager.m_active_screens->actual_count > 0;
	if (cursor_shown != screen_active)
	{
		cursor_shown = screen_active;
		ShowCursor(cursor_shown ? TRUE : FALSE);
	}

	POINT cursor_position{};
	GetCursorPos(&cursor_position);
	ScreenToClient(g_windows_params.game_window_handle, &cursor_position);

	long x_delta = abs(cursor_position.x - user_interface_mouse_globals.update_cursor_position.x);
	long y_delta = abs(cursor_position.y - user_interface_mouse_globals.update_cursor_position.y);
	user_interface_mouse_globals.update_cursor_position.x = cursor_position.x;
	user_interface_mouse_globals.update_cursor_position.y = cursor_position.y;

	static real mouse_move_delta = 0.0f;
	mouse_move_delta += ((real(y_delta * y_delta + x_delta * x_delta) * 1.0f) / 60.0f);

	if (mouse_move_delta > 0.00001f)
		mouse_move_delta *= 0.4f;
	else
		mouse_move_delta = 0.0f;

	if (mouse_move_delta < 0.01f)
		user_interface_mouse_globals.mouse_move_ticks = 0;
	else if (user_interface_mouse_globals.mouse_move_ticks != 255)
		user_interface_mouse_globals.mouse_move_ticks++;
}

void user_interface_mouse_update_internal()
{
	user_interface_mouse_update_tracking();

	user_interface_mouse_globals.controller_index = _controller_index0;
	real x = real(c_rasterizer::render_globals.resolution_offset_x);
	real y = real(c_rasterizer::render_globals.resolution_offset_y);
	real width = real(c_rasterizer::render_globals.window_width24);
	real height = real(c_rasterizer::render_globals.window_height28);

	user_interface_mouse_globals.window_bounds.x0 = x;
	user_interface_mouse_globals.window_bounds.x1 = width - x;
	user_interface_mouse_globals.window_bounds.y0 = y;
	user_interface_mouse_globals.window_bounds.y1 = height - y;

	user_interface_mouse_globals.last_window_cursor_position = user_interface_mouse_globals.window_cursor_position;
	user_interface_mouse_globals.window_cursor_position.x = real(user_interface_mouse_globals.update_cursor_position.x);
	user_interface_mouse_globals.window_cursor_position.y = real(user_interface_mouse_globals.update_cursor_position.y);

	user_interface_mouse_globals.left_button_frames_down = input_mouse_frames_down(_mouse_button_left_click, _input_type_ui);
	user_interface_mouse_globals.right_button_frames_down = input_mouse_frames_down(_mouse_button_right_click, _input_type_ui);

	user_interface_mouse_globals.mouse_wheel_delta = input_globals.raw_mouse_state.wheel_delta;
	user_interface_mouse_globals.mouse_hwheel_delta = input_globals.raw_mouse_state.hwheel_delta;
}

void user_interface_mouse_compute_widget_bounds(c_gui_widget* widget, real_rectangle2d* accumulated_bounds, bool(*child_filter)(c_gui_widget const*))
{
	real_rectangle2d bounds{};
	for (c_gui_widget* child_widget = WIDGET_GET_FIRST_CHILD(widget);
		child_widget;
		child_widget = WIDGET_GET_NEXT_CHILD(child_widget))
	{
		if (child_filter(child_widget))
		{
			csmemset(&bounds, 0, sizeof(bounds));
			child_widget->get_current_bounds(&bounds);

			if (accumulated_bounds->x0 > bounds.x0 || accumulated_bounds->x0 == 0.0f)
				accumulated_bounds->x0 = bounds.x0;

			if (bounds.x1 > accumulated_bounds->x1 || accumulated_bounds->x1 == 0.0f)
				accumulated_bounds->x1 = bounds.x1;

			if (accumulated_bounds->y0 > bounds.y0 || accumulated_bounds->y0 == 0.0f)
				accumulated_bounds->y0 = bounds.y0;

			if (bounds.y1 > accumulated_bounds->y1 || accumulated_bounds->y1 == 0.0f)
				accumulated_bounds->y1 = bounds.y1;
		}
	}
}

void user_interface_mouse_compute_list_item_bounds(c_gui_list_item_widget* list_item_widget, real_rectangle2d* bounds)
{
	user_interface_mouse_compute_widget_bounds(list_item_widget, bounds, [](c_gui_widget const* widget) -> bool { return widget->m_visible && widget->m_type == _gui_widget_type_bitmap; });

	real_point2d anchor_position{};
	anchor_position.x = list_item_widget->m_full_animation_state.__unknown48.x;
	anchor_position.y = list_item_widget->m_full_animation_state.__unknown48.y;

	bounds->x0 += anchor_position.x;
	bounds->x1 += anchor_position.x;
	bounds->y0 += anchor_position.y;
	bounds->y1 += anchor_position.y;
}

bool user_interface_mouse_handle_spinner_list_widget_focus(c_gui_screen_widget* screen_widget, c_gui_list_widget* list_widget)
{
	c_gui_widget* focused_widget = screen_widget->get_focused_widget();
	if (list_widget != focused_widget->get_parent_list())
		return false;

	if (!list_widget->get_parent_list())
		return false;

	if (!list_widget->get_parent_list_item())
		return false;

	if (user_interface_mouse_globals.left_button_frames_down == 1)
		event_manager_button_pressed(user_interface_mouse_globals.controller_index, _button_action_a);

	c_gui_data* data = screen_widget->get_data(list_widget->m_datasource_name.get_value(), NULL);
	long current_item_count = data->get_current_item_count();
	if (!current_item_count)
		return true;

	long index = list_widget->m_focused_item_index - (user_interface_mouse_globals.mouse_wheel_delta / input_globals.mouse_wheel_delta);
	long focused_item_index = index + current_item_count;

	if (index >= 0)
		focused_item_index = index;

	if (focused_item_index >= current_item_count)
		focused_item_index -= current_item_count;

	list_widget->set_focused_item_index(focused_item_index, true);

	return true;
}

void user_interface_mouse_handle_scroll_list_widget(c_gui_screen_widget* screen_widget, c_gui_list_widget* list_widget, long scroll_amount)
{
	if (scroll_amount)
	{
		c_gui_data* data = screen_widget->get_data(list_widget->m_datasource_name.get_value(), NULL);
		long current_item_count = data->get_current_item_count();
		if (current_item_count)
		{
			s_runtime_list_widget_definition* definition = (s_runtime_list_widget_definition*)list_widget->get_core_definition();
			if (list_widget->m_scroll_position != NONE)
			{
				long scroll_position = list_widget->m_scroll_position + scroll_amount;

				if (scroll_position > current_item_count - definition->items.count)
					scroll_position = current_item_count - definition->items.count;

				if (scroll_position < 0)
					scroll_position = 0;

				if (list_widget->m_scroll_position != scroll_position)
					list_widget->m_focused_item_index = NONE;

				list_widget->m_scroll_position = scroll_position;
			}
		}
	}
}

bool user_interface_mouse_handle_list_widget(c_gui_screen_widget* screen_widget, c_gui_list_widget* list_widget)
{
	for (c_gui_list_item_widget* list_item_widget = (c_gui_list_item_widget*)list_widget->get_first_child_widget_by_type(_gui_widget_type_list_item);
		list_item_widget;
		list_item_widget = list_item_widget->get_next_list_item_widget(true))
	{
		if (list_item_widget->m_visible && list_item_widget->get_element_handle() != NONE)
		{
			real_rectangle2d bounds{};
			user_interface_mouse_compute_list_item_bounds(list_item_widget, &bounds);
			widget_bounds_from_window_bounds(&user_interface_mouse_globals.window_bounds, &bounds);

			if (point_intersects_rectangle2d(&user_interface_mouse_globals.window_cursor_position, &bounds))
			{
				c_gui_list_item_widget* focused_widget = static_cast<c_gui_list_item_widget*>(screen_widget->get_focused_widget());

				if (user_interface_mouse_globals.mouse_move_ticks && user_interface_mouse_globals.last_focused_widget != list_item_widget || user_interface_mouse_globals.list_item_selected_dirty)
				{
					user_interface_mouse_globals.list_item_selected_dirty = false;

					if (!focused_widget || focused_widget->get_parent_list() == list_widget)
					{
						if (focused_widget != list_item_widget)
						{
							user_interface_mouse_globals.last_focused_widget = NULL;
							screen_widget->transfer_focus_to_list(list_widget, list_item_widget->get_element_handle(), true, true);
						}
					}
					else
					{
						user_interface_mouse_globals.last_focused_widget = NULL;
						screen_widget->transfer_focus_to_list(list_widget, list_item_widget->get_element_handle(), true, false);
					}
				}

				REFERENCE_DECLARE(offset_pointer(screen_widget, 0x68), dword, v20);
				bool v19 = v20 >> 3 || v20 >> 5;
				if (user_interface_mouse_globals.left_button_frames_down == 1 && list_item_widget == focused_widget && !v19)
				{
					user_interface_mouse_globals.last_focused_widget = list_item_widget;
					event_manager_button_pressed(user_interface_mouse_globals.controller_index, _button_action_a);
				}
			}
			else if (user_interface_mouse_globals.last_focused_widget == list_item_widget)
			{
				user_interface_mouse_globals.last_focused_widget = NULL;
			}
		}
	}

	return false;
}

bool user_interface_mouse_handle_list_widgets(c_gui_screen_widget* screen_widget, c_gui_widget* parent_widget)
{
	bool result = false;

	if (!parent_widget)
		parent_widget = screen_widget;

	for (c_gui_list_widget* list_widget = (c_gui_list_widget*)WIDGET_GET_FIRST_CHILD(parent_widget);
		list_widget && !result;
		list_widget = (c_gui_list_widget*)WIDGET_GET_NEXT_CHILD(list_widget))
	{
		if (list_widget->m_visible)
		{
			e_gui_widget_type widget_type = list_widget->m_type;
			switch (widget_type)
			{
			case _gui_widget_type_list:
				result = user_interface_mouse_handle_list_widget(screen_widget, list_widget);
				break;
			default:
				result = user_interface_mouse_handle_list_widgets(screen_widget, list_widget);
				break;
			}
		}
	}

	return result;
}

bool user_interface_mouse_handle_screen_widget(c_gui_screen_widget* screen_widget)
{
	if (user_interface_mouse_globals.right_button_frames_down == 1)
	{
		event_manager_button_pressed(user_interface_mouse_globals.controller_index, _button_action_b);
		return true;
	}

	c_gui_widget* focused_widget = screen_widget->get_focused_widget();
	if (focused_widget && focused_widget->m_type == _gui_widget_type_list_item)
	{
		c_gui_list_widget* parent_list_widget = focused_widget->get_parent_list();
		if (parent_list_widget && parent_list_widget->m_type == _gui_widget_type_list)
		{
			if (user_interface_mouse_handle_spinner_list_widget_focus(screen_widget, parent_list_widget))
			{
				user_interface_mouse_globals.list_item_selected_dirty = true;
				return true;
			}
		}
	}

	static c_string_id last_screen_widget_name{};
	if (screen_widget->m_name != last_screen_widget_name)
	{
		last_screen_widget_name = screen_widget->m_name;
		user_interface_mouse_globals.list_item_selected_dirty = true;
	}

	if (focused_widget && focused_widget->m_type == _gui_widget_type_list_item)
	{
		c_gui_list_widget* parent_list_widget = focused_widget->get_parent_list();
		if (parent_list_widget && parent_list_widget->m_type == _gui_widget_type_list)
		{
			long parent_focused_item_index = parent_list_widget->m_focused_item_index;
			user_interface_mouse_handle_scroll_list_widget(screen_widget, parent_list_widget, user_interface_mouse_globals.mouse_wheel_delta / -input_globals.mouse_wheel_delta);

			if (short hscroll_ammount = user_interface_mouse_globals.mouse_hwheel_delta / -input_globals.mouse_wheel_delta)
			{
				int16_point2d point = { .x = hscroll_ammount > 0 ? 0x7FFF : -0x7FFF };
				event_manager_tab(0, user_interface_mouse_globals.controller_index, &point, user_interface_milliseconds(), _controller_component_any_stick);
			}
		}
	}

	return user_interface_mouse_handle_list_widgets(screen_widget, NULL) != false;
}

